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

#include "../../mainwindow.h"
#include "../../ui_mainwindow.h"
#include "tileset_item_box.h"
#include "../../common_features/util.h"
#include "../../data_configs/obj_tilesets.h"
#include "../../data_configs/data_configs.h"
#include "../../tilesets/tilesetitembutton.h"

#include <QMessageBox>

TilesetItemBox::TilesetItemBox(QWidget *parent) :
    QDockWidget(parent)
{

}

void MainWindow::on_Tileset_Item_Box_visibilityChanged(bool visible)
{
    ui->actionTilesetBox->setChecked(visible);
}

void MainWindow::on_actionTilesetBox_triggered(bool checked)
{
        ui->Tileset_Item_Box->setVisible(checked);
        if(checked) ui->Tileset_Item_Box->raise();
}


 // Create and refresh tileset box data
void MainWindow::setTileSetBox()
{
    clearTilesetGroups();
    QStringList filters("*.tsgrp.ini");
    QDir grpDir = configs.config_dir + "group_tilesets/";
    QStringList entries;
    entries = grpDir.entryList(filters, QDir::Files);
    foreach (QString f, entries) {
        SimpleTilesetGroup grp;
        if(!TilesetGroupEditor::OpenSimpleTilesetGroup(configs.config_dir + "group_tilesets/" + f,grp))
            continue;
        prepareTilesetGroup(grp);
    }
}



void MainWindow::on_tilesetGroup_currentIndexChanged(int index)
{
    Q_UNUSED(index);

}



void MainWindow::on_newTileset_clicked()
{
    QMessageBox::information(this, "test", "test", QMessageBox::Ok);
}

QFrame* MainWindow::getFrameTilesetOfTab(QWidget* catTab){
    QList<QFrame*> framechildren = catTab->findChildren<QFrame*>();
    foreach(QFrame* f, framechildren){
        if(qobject_cast<QLabel*>(f)!=0)
            continue;
        return f;
    }
    return 0;
}

QComboBox* MainWindow::getGroupComboboxOfTab(QWidget* catTab){
    return catTab->findChild<QComboBox*>();
}

QWidget* MainWindow::findTabWidget(const QString &categoryItem){
    QTabWidget* cat = ui->TileSetsCategories;
    for(int i = 0; i < cat->count(); ++i){
        if(cat->tabText(i) == categoryItem){
            return cat->widget(i);
        }
    }
    return 0;
}

QWidget* MainWindow::makeCategory(const QString &categoryItem){
    QTabWidget* TileSetsCategories = ui->TileSetsCategories;
    QWidget* catWid;
    QGridLayout* catLayout;
    QLabel* grpLabel;
    QComboBox* tilesetGroup;
    QSpacerItem* spItem;
    QFrame* TileSets;


    catWid = new QWidget();
    catLayout = new QGridLayout(catWid);
    catLayout->setSpacing(6);
    catLayout->setContentsMargins(11, 11, 11, 11);
    grpLabel = new QLabel(catWid);

    catLayout->addWidget(grpLabel, 0, 0, 1, 1);

    tilesetGroup = new QComboBox(catWid);

    catLayout->addWidget(tilesetGroup, 0, 1, 1, 1);

    spItem = new QSpacerItem(1283, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    catLayout->addItem(spItem, 0, 2, 1, 1);

    TileSets = new QFrame(catWid);
    Q_UNUSED(new FlowLayout(TileSets));
    TileSets->setFrameShape(QFrame::StyledPanel);
    TileSets->setFrameShadow(QFrame::Raised);

    catLayout->addWidget(TileSets, 1, 0, 1, 3);

    TileSetsCategories->addTab(catWid, QString());
    TileSetsCategories->setTabText(TileSetsCategories->indexOf(catWid), categoryItem);


    return catWid;
}

void MainWindow::prepareTilesetGroup(const SimpleTilesetGroup &tilesetGroups){
    QWidget *t = findTabWidget(tilesetGroups.groupCat);
    if(!t){
        t = makeCategory(tilesetGroups.groupCat);
    }
    QComboBox *c = getGroupComboboxOfTab(t);
    if(!c)
        return;

    if(!util::contains(c,tilesetGroups.groupName)){
        c->addItem(tilesetGroups.groupName);
    }
}

void MainWindow::clearTilesetGroups(){
    QTabWidget* cat = ui->TileSetsCategories;
    int i = 0;
    int f = cat->count();
    while(f > i){ //include custom tab
        if(cat->tabText(i) == tr("Custom")){
            ++i;
            continue;
        }

        QWidget* acCat = cat->widget(i);
        cat->removeTab(i);
        delete acCat;
        f = cat->count();
    }
}

void MainWindow::makeCurrentTileset(){
    QTabWidget* cat = ui->TileSetsCategories;
    if(!(cat->tabText(cat->currentIndex()) == tr("Custom"))){
        QWidget* current = cat->currentWidget();
        if(!current)
            return;
        QFrame* currentFrame = getFrameTilesetOfTab(current);
        QComboBox* currentCombo = getGroupComboboxOfTab(current);
        if(!currentFrame || !currentCombo)
            return;

        qDeleteAll(currentFrame->findChildren<QGroupBox*>());

        if(currentFrame->layout() == 0)
            currentFrame->setLayout(new FlowLayout());

        QString currentGroup = currentCombo->currentText();
        for(int i = 0; i < configs.main_tilesets_grp.size(); ++i){
            if(configs.main_tilesets_grp[i].groupName == currentGroup){
                QStringList l = configs.main_tilesets_grp[i].tilesets;
                foreach(QString s, l){
                    for(int j = 0; j < configs.main_tilesets.size(); ++j){
                        if(s == configs.main_tilesets[j].fileName){
                            SimpleTileset &s = configs.main_tilesets[j];
                            QGroupBox* tilesetNameWrapper = new QGroupBox(s.tileSetName,currentFrame);
                            ((FlowLayout*)currentFrame->layout())->addWidget(tilesetNameWrapper);
                            QGridLayout* l = new QGridLayout(tilesetNameWrapper);
                            for(int k = 0; k < s.items.size(); ++k){
                                SimpleTilesetItem &item = s.items[k];
                                TilesetItemButton* tbutton = new TilesetItemButton(&configs,tilesetNameWrapper);
                                tbutton->applySize(32,32);
                                tbutton->applyItem(s.type, item.id);
                                l->addWidget(tbutton, item.row, item.col);
                            }

                        }
                    }
                }
            }
        }
    }
}
