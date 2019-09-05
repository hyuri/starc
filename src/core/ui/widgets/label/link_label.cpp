#include "link_label.h"

#include <ui/design_system/design_system.h>

#include <QDesktopServices>
#include <QUrl>


class AbstractLinkLabel::Implementation
{
public:
    QUrl link;
};


// ****


AbstractLinkLabel::AbstractLinkLabel(QWidget* _parent)
    : AbstractLabel(_parent),
      d(new Implementation)
{
    setCursor(Qt::PointingHandCursor);
}

AbstractLinkLabel::~AbstractLinkLabel() = default;

void AbstractLinkLabel::setLink(const QUrl& _link)
{
    if (d->link == _link) {
        return;
    }

    d->link = _link;
}

void AbstractLinkLabel::mouseReleaseEvent(QMouseEvent* _event)
{
    Q_UNUSED(_event);

    if (d->link.isEmpty()) {
        return;
    }

    QDesktopServices::openUrl(d->link);
}


// ****


Body1LinkLabel::Body1LinkLabel(QWidget* _parent)
    : AbstractLinkLabel(_parent)
{
}

const QFont& Body1LinkLabel::textFont() const
{
    static QFont font;
    if (font.pixelSize() != Ui::DesignSystem::font().body1().pixelSize()) {
        font = Ui::DesignSystem::font().body1();
        font.setUnderline(true);
    }
    return font;
}
