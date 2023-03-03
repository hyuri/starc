#pragma once

#include <QAbstractItemModel>


namespace BusinessLayer {

class ScreenplayTextModel;

/**
 * @brief Модель списка персонажей и сцен, где они участвуют
 */
class ScreenplayBreakdownStructureLocationsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ScreenplayBreakdownStructureLocationsModel(QObject* _parent = nullptr);
    ~ScreenplayBreakdownStructureLocationsModel() override;

    /**
     * @brief Задать модель текста сценария
     */
    void setSourceModel(BusinessLayer::ScreenplayTextModel* _model);

    /**
     * @brief Задать текущий элемент модели текста сценария
     */
    void setSourceModelCurrentIndex(const QModelIndex& _index);

    /**
     * @brief Реализация базовых вещей для древовидной модели
     */
    /** @{ */
    QModelIndex index(int _row, int _column, const QModelIndex& _parent = {}) const override;
    QModelIndex parent(const QModelIndex& _child) const override;
    int columnCount(const QModelIndex& _parent = {}) const override;
    int rowCount(const QModelIndex& _parent = {}) const override;
    Qt::ItemFlags flags(const QModelIndex& _index) const override;
    QVariant data(const QModelIndex& _index, int _role) const override;
    /** @} */

private:
    class Implementation;
    QScopedPointer<Implementation> d;
};

} // namespace BusinessLayer
