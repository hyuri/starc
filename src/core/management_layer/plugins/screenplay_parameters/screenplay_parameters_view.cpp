#include "screenplay_parameters_view.h"

#include <business_layer/templates/screenplay_template.h>
#include <business_layer/templates/templates_facade.h>
#include <data_layer/storage/settings_storage.h>
#include <data_layer/storage/storage_facade.h>
#include <ui/design_system/design_system.h>
#include <ui/widgets/card/card.h>
#include <ui/widgets/check_box/check_box.h>
#include <ui/widgets/combo_box/combo_box.h>
#include <ui/widgets/scroll_bar/scroll_bar.h>
#include <ui/widgets/text_field/text_field.h>

#include <QGridLayout>
#include <QScrollArea>
#include <QStandardItemModel>


namespace Ui {

class ScreenplayParametersView::Implementation
{
public:
    explicit Implementation(QWidget* _parent);


    QScrollArea* content = nullptr;

    Card* screenplayInfo = nullptr;
    QGridLayout* infoLayout = nullptr;
    TextField* header = nullptr;
    CheckBox* printHeaderOnTitlePage = nullptr;
    TextField* footer = nullptr;
    CheckBox* printFooterOnTitlePage = nullptr;
    TextField* scenesNumbersPrefix = nullptr;
    TextField* scenesNumberingStartAt = nullptr;
    CheckBox* overrideCommonSettings = nullptr;
    ComboBox* screenplayTemplate = nullptr;
    CheckBox* showSceneNumbers = nullptr;
    CheckBox* showSceneNumbersOnLeft = nullptr;
    CheckBox* showSceneNumbersOnRight = nullptr;
    CheckBox* showDialoguesNumbers = nullptr;
};

ScreenplayParametersView::Implementation::Implementation(QWidget* _parent)
    : content(new QScrollArea(_parent))
    , screenplayInfo(new Card(_parent))
    , infoLayout(new QGridLayout)
    , header(new TextField(screenplayInfo))
    , printHeaderOnTitlePage(new CheckBox(screenplayInfo))
    , footer(new TextField(screenplayInfo))
    , printFooterOnTitlePage(new CheckBox(screenplayInfo))
    , scenesNumbersPrefix(new TextField(screenplayInfo))
    , scenesNumberingStartAt(new TextField(screenplayInfo))
    , overrideCommonSettings(new CheckBox(screenplayInfo))
    , screenplayTemplate(new ComboBox(_parent))
    , showSceneNumbers(new CheckBox(_parent))
    , showSceneNumbersOnLeft(new CheckBox(_parent))
    , showSceneNumbersOnRight(new CheckBox(_parent))
    , showDialoguesNumbers(new CheckBox(_parent))
{
    QPalette palette;
    palette.setColor(QPalette::Base, Qt::transparent);
    palette.setColor(QPalette::Window, Qt::transparent);
    content->setPalette(palette);
    content->setFrameShape(QFrame::NoFrame);
    content->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    content->setVerticalScrollBar(new ScrollBar);

    header->setSpellCheckPolicy(SpellCheckPolicy::Manual);
    footer->setSpellCheckPolicy(SpellCheckPolicy::Manual);
    scenesNumbersPrefix->setSpellCheckPolicy(SpellCheckPolicy::Manual);
    scenesNumberingStartAt->setSpellCheckPolicy(SpellCheckPolicy::Manual);

    screenplayTemplate->setSpellCheckPolicy(SpellCheckPolicy::Manual);
    screenplayTemplate->setModel(BusinessLayer::TemplatesFacade::screenplayTemplates());
    screenplayTemplate->hide();

    showSceneNumbers->hide();
    showSceneNumbersOnLeft->setEnabled(false);
    showSceneNumbersOnLeft->hide();
    showSceneNumbersOnRight->setEnabled(false);
    showSceneNumbersOnRight->hide();
    showDialoguesNumbers->hide();

    infoLayout->setContentsMargins({});
    infoLayout->setSpacing(0);
    int row = 0;
    infoLayout->setRowMinimumHeight(row++, 1); // добавляем пустую строку сверху
    infoLayout->addWidget(header, row++, 0);
    infoLayout->addWidget(printHeaderOnTitlePage, row++, 0);
    infoLayout->addWidget(footer, row++, 0);
    infoLayout->addWidget(printFooterOnTitlePage, row++, 0);
    infoLayout->addWidget(scenesNumbersPrefix, row++, 0);
    infoLayout->addWidget(scenesNumberingStartAt, row++, 0);
    infoLayout->addWidget(overrideCommonSettings, row++, 0);
    infoLayout->addWidget(screenplayTemplate, row++, 0);
    {
        auto layout = new QHBoxLayout;
        layout->setContentsMargins({});
        layout->setSpacing(0);
        layout->addWidget(showSceneNumbers);
        layout->addWidget(showSceneNumbersOnLeft);
        layout->addWidget(showSceneNumbersOnRight);
        layout->addStretch();
        infoLayout->addLayout(layout, row++, 0);
    }
    infoLayout->addWidget(showDialoguesNumbers, row++, 0);
    infoLayout->setRowMinimumHeight(row++, 1); // добавляем пустую строку внизу
    infoLayout->setColumnStretch(0, 1);
    screenplayInfo->setLayoutReimpl(infoLayout);

    QWidget* contentWidget = new QWidget;
    content->setWidget(contentWidget);
    content->setWidgetResizable(true);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins({});
    layout->setSpacing(0);
    layout->addWidget(screenplayInfo);
    layout->addStretch();
    contentWidget->setLayout(layout);
}


// ****


ScreenplayParametersView::ScreenplayParametersView(QWidget* _parent)
    : Widget(_parent)
    , d(new Implementation(this))
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins({});
    layout->setSpacing(0);
    layout->addWidget(d->content);
    setLayout(layout);

    connect(d->header, &TextField::textChanged, this,
            [this] { emit headerChanged(d->header->text()); });
    connect(d->printHeaderOnTitlePage, &CheckBox::checkedChanged, this,
            &ScreenplayParametersView::printHeaderOnTitlePageChanged);
    connect(d->footer, &TextField::textChanged, this,
            [this] { emit footerChanged(d->footer->text()); });
    connect(d->printFooterOnTitlePage, &CheckBox::checkedChanged, this,
            &ScreenplayParametersView::printFooterOnTitlePageChanged);
    connect(d->scenesNumbersPrefix, &TextField::textChanged, this,
            [this] { emit scenesNumbersPrefixChanged(d->scenesNumbersPrefix->text()); });
    connect(d->scenesNumberingStartAt, &TextField::textChanged, this, [this] {
        bool isNumberValid = false;
        const auto startNumber = d->scenesNumberingStartAt->text().toInt(&isNumberValid);
        if (isNumberValid) {
            emit scenesNumberingStartAtChanged(startNumber);
        } else {
            d->scenesNumberingStartAt->undo();
        }
    });
    connect(d->overrideCommonSettings, &CheckBox::checkedChanged, this,
            &ScreenplayParametersView::overrideCommonSettingsChanged);
    connect(d->screenplayTemplate, &ComboBox::currentIndexChanged, this,
            [this](const QModelIndex& _index) {
                const auto templateId
                    = _index.data(BusinessLayer::TemplatesFacade::kTemplateIdRole).toString();
                emit screenplayTemplateChanged(templateId);
            });
    connect(d->showSceneNumbers, &CheckBox::checkedChanged, this,
            &ScreenplayParametersView::showSceneNumbersChanged);
    connect(d->showSceneNumbersOnLeft, &CheckBox::checkedChanged, this,
            &ScreenplayParametersView::showSceneNumbersOnLeftChanged);
    connect(d->showSceneNumbersOnRight, &CheckBox::checkedChanged, this,
            &ScreenplayParametersView::showSceneNumbersOnRightChanged);
    connect(d->showDialoguesNumbers, &CheckBox::checkedChanged, this,
            &ScreenplayParametersView::showDialoguesNumbersChanged);

    connect(d->overrideCommonSettings, &CheckBox::checkedChanged, this, [this](bool _checked) {
        using namespace BusinessLayer;

        //
        // При включении перезаписи параметров, настроим их также, как и в общих параметрах
        //
        if (_checked) {
            //
            // Шаблон
            //
            for (int row = 0; row < TemplatesFacade::screenplayTemplates()->rowCount(); ++row) {
                const auto item = TemplatesFacade::screenplayTemplates()->item(row);
                if (item->data(TemplatesFacade::kTemplateIdRole).toString()
                    != TemplatesFacade::screenplayTemplate().id()) {
                    continue;
                }

                d->screenplayTemplate->setCurrentIndex(item->index());
                break;
            }

            //
            // Остальные
            //
            auto settingsValue = [](const QString& _key) {
                return DataStorageLayer::StorageFacade::settingsStorage()->value(
                    _key, DataStorageLayer::SettingsStorage::SettingsPlace::Application);
            };
            d->showSceneNumbers->setChecked(
                settingsValue(DataStorageLayer::kComponentsScreenplayEditorShowSceneNumbersKey)
                    .toBool());
            d->showSceneNumbersOnLeft->setChecked(
                settingsValue(DataStorageLayer::kComponentsScreenplayEditorShowSceneNumberOnLeftKey)
                    .toBool());
            d->showSceneNumbersOnRight->setChecked(
                settingsValue(
                    DataStorageLayer::kComponentsScreenplayEditorShowSceneNumbersOnRightKey)
                    .toBool());
            d->showDialoguesNumbers->setChecked(
                settingsValue(DataStorageLayer::kComponentsScreenplayEditorShowDialogueNumberKey)
                    .toBool());
        }

        d->screenplayTemplate->setVisible(_checked);
        d->showSceneNumbers->setVisible(_checked);
        d->showSceneNumbersOnLeft->setVisible(_checked);
        d->showSceneNumbersOnRight->setVisible(_checked);
        d->showDialoguesNumbers->setVisible(_checked);
    });
    connect(d->showSceneNumbers, &CheckBox::checkedChanged, d->showSceneNumbersOnLeft,
            &CheckBox::setEnabled);
    connect(d->showSceneNumbers, &CheckBox::checkedChanged, d->showSceneNumbersOnRight,
            &CheckBox::setEnabled);
    auto correctShownSceneNumber = [this] {
        if (!d->showSceneNumbersOnLeft->isChecked() && !d->showSceneNumbersOnRight->isChecked()) {
            d->showSceneNumbersOnLeft->setChecked(true);
        }
    };
    connect(d->showSceneNumbersOnLeft, &CheckBox::checkedChanged, this, correctShownSceneNumber);
    connect(d->showSceneNumbersOnRight, &CheckBox::checkedChanged, this, correctShownSceneNumber);

    updateTranslations();
    designSystemChangeEvent(nullptr);
}

ScreenplayParametersView::~ScreenplayParametersView() = default;


void ScreenplayParametersView::setHeader(const QString& _header)
{
    d->header->setText(_header);
}

void ScreenplayParametersView::setPrintHeaderOnTitlePage(bool _print)
{
    d->printHeaderOnTitlePage->setChecked(_print);
}

void ScreenplayParametersView::setFooter(const QString& _footer)
{
    d->footer->setText(_footer);
}

void ScreenplayParametersView::setPrintFooterOnTitlePage(bool _print)
{
    d->printFooterOnTitlePage->setChecked(_print);
}

void ScreenplayParametersView::setScenesNumbersPrefix(const QString& _prefix)
{
    d->scenesNumbersPrefix->setText(_prefix);
}

void ScreenplayParametersView::setScenesNumbersingStartAt(int _startNumber)
{
    const auto startNumberText = QString::number(_startNumber);
    if (d->scenesNumberingStartAt->text() == startNumberText) {
        return;
    }

    d->scenesNumberingStartAt->setText(startNumberText);
}

void ScreenplayParametersView::setOverrideCommonSettings(bool _override)
{
    d->overrideCommonSettings->setChecked(_override);
}

void ScreenplayParametersView::setScreenplayTemplate(const QString& _templateId)
{
    using namespace BusinessLayer;
    for (int row = 0; row < TemplatesFacade::screenplayTemplates()->rowCount(); ++row) {
        auto item = TemplatesFacade::screenplayTemplates()->item(row);
        if (item->data(TemplatesFacade::kTemplateIdRole).toString() != _templateId) {
            continue;
        }

        d->screenplayTemplate->setCurrentIndex(item->index());
        break;
    }
}

void ScreenplayParametersView::setShowSceneNumbers(bool _show)
{
    d->showSceneNumbers->setChecked(_show);
}

void ScreenplayParametersView::setShowSceneNumbersOnLeft(bool _show)
{
    d->showSceneNumbersOnLeft->setChecked(_show);
}

void ScreenplayParametersView::setShowSceneNumbersOnRight(bool _show)
{
    d->showSceneNumbersOnRight->setChecked(_show);
}

void ScreenplayParametersView::setShowDialoguesNumbers(bool _show)
{
    d->showDialoguesNumbers->setChecked(_show);
}

void ScreenplayParametersView::updateTranslations()
{
    d->header->setLabel(tr("Header"));
    d->printHeaderOnTitlePage->setText(tr("Print header on title page"));
    d->footer->setLabel(tr("Footer"));
    d->printFooterOnTitlePage->setText(tr("Print footer on title page"));
    d->scenesNumbersPrefix->setLabel(tr("Scenes numbers' prefix"));
    d->scenesNumberingStartAt->setLabel(tr("Scenes numbering start at"));
    d->overrideCommonSettings->setText(tr("Override common settings for this screenplay"));
    d->screenplayTemplate->setLabel(tr("Template"));
    d->showSceneNumbers->setText(tr("Print scenes numbers"));
    d->showSceneNumbersOnLeft->setText(tr("on the left"));
    d->showSceneNumbersOnRight->setText(tr("on the right"));
    d->showDialoguesNumbers->setText(tr("Print dialogues numbers"));
}

void ScreenplayParametersView::designSystemChangeEvent(DesignSystemChangeEvent* _event)
{
    Widget::designSystemChangeEvent(_event);

    setBackgroundColor(Ui::DesignSystem::color().surface());

    d->content->widget()->layout()->setContentsMargins(
        QMarginsF(Ui::DesignSystem::layout().px24(), Ui::DesignSystem::layout().topContentMargin(),
                  Ui::DesignSystem::layout().px24(), Ui::DesignSystem::layout().px24())
            .toMargins());

    d->screenplayInfo->setBackgroundColor(DesignSystem::color().background());
    for (auto textField : std::vector<TextField*>{
             d->header,
             d->footer,
             d->scenesNumbersPrefix,
             d->scenesNumberingStartAt,
             d->screenplayTemplate,
         }) {
        textField->setBackgroundColor(Ui::DesignSystem::color().onBackground());
        textField->setTextColor(Ui::DesignSystem::color().onBackground());
    }
    for (auto checkBox : {
             d->printHeaderOnTitlePage,
             d->printFooterOnTitlePage,
             d->overrideCommonSettings,
             d->showSceneNumbers,
             d->showSceneNumbersOnLeft,
             d->showSceneNumbersOnRight,
             d->showDialoguesNumbers,
         }) {
        checkBox->setBackgroundColor(Ui::DesignSystem::color().background());
        checkBox->setTextColor(Ui::DesignSystem::color().onBackground());
    }
    d->infoLayout->setVerticalSpacing(static_cast<int>(Ui::DesignSystem::layout().px16()));
    d->infoLayout->setRowMinimumHeight(0, static_cast<int>(Ui::DesignSystem::layout().px24()));
    d->infoLayout->setRowMinimumHeight(d->infoLayout->rowCount() - 1,
                                       static_cast<int>(Ui::DesignSystem::layout().px12()));
}

} // namespace Ui
