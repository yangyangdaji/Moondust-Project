/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TILESETCONFIGUREDIALOG_H
#define TILESETCONFIGUREDIALOG_H

#include "tileset.h"
#include "piecesmodel.h"
#include "../data_configs/data_configs.h"

#include <QDialog>

namespace Ui {
class TilesetConfigureDialog;
}

class TilesetConfigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TilesetConfigureDialog(dataconfigs* conf, QWidget *parent = 0);
    ~TilesetConfigureDialog();



private slots:
    void on_pushButton_clicked();
    void setUpItems(int type);
    void setUpTileset(int type);
private:
    Ui::TilesetConfigureDialog *ui;
    tileset* m_tileset;
    PiecesModel* m_model;
    dataconfigs* m_conf;
    void setUpItems(tileset::TilesetType type);
    PiecesModel::PieceType toPieceType(tileset::TilesetType type);
};

#endif // TILESETCONFIGUREDIALOG_H
