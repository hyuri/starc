#pragma once

#include <business_layer/model/text/text_model_folder_item.h>

#include <QPair>
#include <QRectF>

#include <chrono>


namespace BusinessLayer {

class NovelTextModel;

/**
 * @brief Класс элементов папок модели сценария
 */
class CORE_LIBRARY_EXPORT NovelTextModelFolderItem : public TextModelFolderItem
{
public:
    /**
     * @brief Роли данных из модели
     */
    enum {
        FolderDurationRole = TextModelFolderItem::FolderUserRole + 1,
    };

public:
    explicit NovelTextModelFolderItem(const NovelTextModel* _model, TextFolderType _type);
    ~NovelTextModelFolderItem() override;

    /**
     * @brief Параметры отображения карточки
     */
    struct CardInfo {
        QRectF geometry;
        bool isOpened = false;
    };
    const CardInfo& cardInfo() const;
    void setCardInfo(const CardInfo& _info);

    /**
     * @brief Количество слов
     */
    int wordsCount() const;

    /**
     * @brief Количество символов
     */
    QPair<int, int> charactersCount() const;

    /**
     * @brief Подходит ли элемент под условия заданного фильтра
     */
    bool isFilterAccepted(const QString& _text, bool _isCaseSensitive,
                          int _filterType) const override;

protected:
    /**
     * @brief Обновляем текст папки при изменении кого-то из детей
     */
    void handleChange() override;

private:
    class Implementation;
    QScopedPointer<Implementation> d;
};

} // namespace BusinessLayer
