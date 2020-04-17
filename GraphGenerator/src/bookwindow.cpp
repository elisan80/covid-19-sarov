/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "bookwindow.h"
#include "bookdelegate.h"
#include "initdb.h"

#include <QtSql>

BookWindow::BookWindow()
{
    ui.setupUi(this);

    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QMessageBox::critical(
                    this,
                    "Unable to load database",
                    "This program needs the SQLITE driver"
                    );

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
     db.setDatabaseName(":memory:");

     if (!db.open())
         return;

       // Create the data model:
    model = new QSqlRelationalTableModel(ui.bookTable);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //model->setTable("humans");

    // Set the localized header captions:
    model->setHeaderData(model->fieldIndex("human"), Qt::Horizontal, tr("human"));
    model->setHeaderData(model->fieldIndex("home"), Qt::Horizontal, tr("home"));
    model->setHeaderData(model->fieldIndex("age"), Qt::Horizontal, tr("age"));
    model->setHeaderData(model->fieldIndex("job_sites"), Qt::Horizontal, tr("job_sites"));
    model->setHeaderData(model->fieldIndex("holiday_sites"), Qt::Horizontal, tr("holiday_sites"));


    // Set the model and hide the ID column:
    ui.bookTable->setModel(model);
    ui.bookTable->setItemDelegate(new BookDelegate(ui.bookTable));
    //ui.bookTable->setColumnHidden(model->fieldIndex("id"), true);
    ui.bookTable->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui.saveHumanBase, &QPushButton::clicked,
            this, &BookWindow::saveHumanBase);
    connect(ui.genHumansDat, &QPushButton::clicked,
            this, &BookWindow::generateHumansFile);
    connect(ui.generateDBHuman, &QPushButton::clicked,
            this, &BookWindow::generateDBHuman);
    connect(ui.loadHumanBase, &QPushButton::clicked,
            this, &BookWindow::loadHumanBase);

   /* ui.bookTable->setCurrentIndex(model->index(0, 0));*/
    createMenuBar();
}

void BookWindow::generateDBHuman()
{
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QMessageBox::critical(
                    this,
                    "Unable to load database",
                    "This demo needs the SQLITE driver"
                    );

    // Initialize the database:
    int seed;
    if(ui.lineEditSeed->text().isEmpty())
        seed = 37;
    else
        seed = ui.lineEditSeed->text().toInt();

    QSqlError err = initDb(seed);
    if (err.type() != QSqlError::NoError)
    {
        showError(err);
        return;
    }


    ui.bookTable->setColumnHidden(model->fieldIndex("id"), true);
    model->setTable("humans");

    // Populate the model:
    if (!model->select()) {
        showError(model->lastError());
        return;
    }
    while(model->canFetchMore())
            model->fetchMore();

    ui.bookTable->setCurrentIndex(model->index(0, 0));
}
void BookWindow::deleteAllRows()
{
    while(model->rowCount() > 1000)
    {
        model->removeRows(0,1000);
        model->submitAll();
        while(model->canFetchMore())
                model->fetchMore();
    }
    model->removeRows(0,model->rowCount());
    model->submitAll();
    while(model->canFetchMore())
            model->fetchMore();
}


void BookWindow::loadHumanBase()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Human base and associated job graph"), "",
        tr("(*.txt);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else
    {
        //чтение БД города
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QSqlDatabase db = QSqlDatabase::database();//дефолтное соединение

        if (!db.open())
            return;

        QSqlQuery q;
        QStringList tables = db.tables();
        if (tables.contains("humans", Qt::CaseInsensitive))
            q.exec(HUMANS_DEL_SQL);
            //return;

        if (!q.exec(HUMANS_SQL))
            return;

        if (!q.prepare(INSERT_HUMAN_SQL))
            return;

        QModelIndex idx;
        QTextStream in(&file);
        QStringList humanStringLst;
        QStringList props;

        //while (!in.atEnd())
        QString alf = in.readAll();
        QStringList alflst = alf.split("\n");

        int cur_human;
        int current_home;
        int current_age;
        QString current_job;
        QString current_holiday;

        foreach(auto line, alflst )
        {
            if (!line.isEmpty())
            {
                humanStringLst = line.split(" ");
                current_job.clear();
                foreach(auto rec, humanStringLst )
                {
                    if(!rec.isEmpty())
                    {
                        props = rec.split("=");
                        if (props.at(0) == "human")
                            cur_human = props.at(1).toInt();
                        else if (props.at(0) == "home")
                            current_home = props.at(1).toInt();
                        else if (props.at(0) == "age")
                            current_age = props.at(1).toInt();
                        else if (props.at(0) == "job_sites")
                            current_job = props.at(1);
                        else if (props.at(0) == "holiday_sites")
                            current_holiday = props.at(1);
                    }
                }
                addRecord(q, cur_human, current_home, current_age, current_job, current_holiday);
            }
        }
        model->setTable("humans");

        // Populate the model:
        if (!model->select())
        {
            showError(model->lastError());
            return;
        }
        while(model->canFetchMore())
                model->fetchMore();


        //чтение графа
        QFileInfo fin(file);
        QString jobName = fin.absolutePath() + "/" + fin.baseName() + "JobGraph.gv";

        QFile graph_file(jobName);
        if (!graph_file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                graph_file.errorString());
            return;
        }
        QTextStream graph_stream(&graph_file);
        QString str_from_graph;
        str_from_graph = graph_stream.readLine();
        if(!str_from_graph.contains("digraph") || !str_from_graph.contains("JOB_HIERARHY"))
            QMessageBox::information(this, tr("Can not read file. Forbidden format"),
                graph_file.errorString());

        while(!str_from_graph.contains("->"))
            str_from_graph = graph_stream.readLine();

        jobGraphToUp.clear();
        jobGraphToDown.clear();
        QStringList sl1,sl2,sl3;
        QString id1, id2;
        while(str_from_graph.contains("->"))
        {
            sl1 = str_from_graph.split("[");
            sl2 = sl1.at(0).split(" -> ");
            id1 = sl2.at(0);
            id2 = sl2.at(1);
            if(sl1.at(1).contains("solid"))
            {//add to jobGraphToUp
                jobGraphToUp.insert(id1, id2);
            }
            if(sl1.at(1).contains("dashed"))
            {//add to jobGraphToDown
                jobGraphToDown.insertMulti(id1, id2);
            }
            str_from_graph = graph_stream.readLine();
        }


    }
}
void BookWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                "Error initializing database: " + err.text());
}

void BookWindow::createMenuBar()
{
    QAction *quitAction = new QAction(tr("&Quit"), this);
    QAction *aboutAction = new QAction(tr("&About"), this);
    QAction *aboutQtAction = new QAction(tr("&About Qt"), this);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(quitAction);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    connect(quitAction, &QAction::triggered, this, &BookWindow::close);
    connect(aboutAction, &QAction::triggered, this, &BookWindow::about);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void BookWindow::saveHumanBase()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save config file"), "",
        tr("config (*.txt);;All Files (*)"));//"G:\\covid\\db_human.txt";

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        QTextStream out(&file);
        QModelIndex myIndex;
        QVariant myData;
        QString oneString;
        QString dataStr;         
        QFileInfo fin(file);
        QString pathAndName = fin.absolutePath() + "/" + fin.baseName();
        //out.setVersion(QDataStream::Qt_4_5);
        for(int i=0;i<model->rowCount();i++)
        {
            oneString.clear();
            for (int j=1;j<model->columnCount();j++)
            {
               myIndex = model->index(i,j);
               myData = model->data(myIndex,Qt::DisplayRole);
               dataStr = myData.toString();
               if(!dataStr.isEmpty()) oneString += model->headerData(j,Qt::Horizontal).toString() + "=" + dataStr + " ";
            }
            out << oneString << "\n";
        }
        saveJobGraph(pathAndName);
    }
}

void BookWindow::generateHumansFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("config file"), "",
            tr("config (*.dat);;All Files (*)"));"G:\\covid\\humans.dat";

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream out(&file);
        QModelIndex myIndex;
        QVariant myData;
        QString oneString;
        QString dataStr, holidayStr;
        QString humanIdStr;
        QString homeStr, jobStr;
        QString chiefStr;

        bool isHoliday =  ui.radioButtonHoliday->isChecked();

        int genseed = ui.lineEditSeed->text().toInt();
        QRandomGenerator rg = QRandomGenerator();
        rg.seed(genseed);

        for(int i=0;i<model->rowCount();i++)
        {
            oneString.clear();
            humanIdStr = model->data(model->index(i,1),Qt::DisplayRole).toString();//id
            homeStr = model->data(model->index(i,2),Qt::DisplayRole).toString();//home

            jobStr = model->data(model->index(i,4),Qt::DisplayRole).toString();//job
            int jobPlace = jobStr.toInt();

            holidayStr = model->data(model->index(i,5),Qt::DisplayRole).toString();//holiday_sites
            QStringList dataList = holidayStr.split(";");
            int numShopsPerOneHuman = dataList.size()-1;
            double probGoToEachShop = ui.ProbGoToShop->text().toDouble()/numShopsPerOneHuman;

            oneString = "[human_id=" +  humanIdStr + " home_id=" + homeStr + "]\n";
            oneString += "{\n";

            if(isHoliday)
            {
                if(jobPlace >= ShopBeginIndex && jobPlace < ShopBeginIndex+ui.ShopNum->text().toInt())
                {
                     oneString += jobStr + " 08:00 22:00 50400 50400 -1 \n";
                }
                else
                {
                    foreach (auto place, dataList)
                    {
                       if(!place.isEmpty())
                       {
                           oneString += place + " 08:00 22:00 1800 3600 " + QString::number(probGoToEachShop) + "\n";
                       }
                    }
                }
            }
            else
            {
                if(!jobStr.isEmpty())
                {
                    int probToWork = rg.bounded(0,100);
                    bool working = false;
                    int workPercent = 20;
                    if(!ui.lineEditWorkPercent->text().isEmpty()) workPercent = ui.lineEditWorkPercent->text().toInt();
                    if(probToWork < workPercent) working = true;

                    if(jobPlace >= ShopBeginIndex && jobPlace < ShopBeginIndex+ui.ShopNum->text().toInt())
                    {
                         oneString += jobStr + " 08:00 22:00 50400 50400 -1 \n";
                    }
                    else if(working)
                    {
                        oneString += jobStr + " 08:00 14:00 600 33120 -1 \n";

                        chiefStr = jobGraphToUp.value(jobStr);
                        if(!chiefStr.isEmpty())
                        {
                            oneString += chiefStr + " 08:00 14:00 600 3600 0.2 \n";
                        }
                        QStringList places = getNeighboursJobPlaces(jobStr);//архив и прочая...

                        foreach (auto place, places)
                        {
                           if(!place.isEmpty())
                              oneString += place + " 08:00 14:00 2000 3600 0.2 \n";
                        }
                    }
                    else
                    {
                        foreach (auto place, dataList)
                        {
                           if(!place.isEmpty())
                           {
                               oneString += place + " 08:00 14:00 1800 3600 " + QString::number(probGoToEachShop) + "\n";
                           }
                        }
                    }
                }
                else
                {
                    foreach (auto place, dataList)
                    {
                       if(!place.isEmpty())
                       {
                           oneString += place + " 08:00 14:00 1800 3600 " + QString::number(probGoToEachShop) + "\n";
                       }
                    }
                }
                foreach (auto place, dataList)
                {
                   if(!place.isEmpty())
                   {
                       oneString += place + " 14:00 22:00 1800 3600 " + QString::number(probGoToEachShop) + "\n";
                   }
                }
            }
            oneString += "}\n";
            out << oneString;
        }
    }
}

void BookWindow::about()
{
    QMessageBox::about(this, tr("About Books"),
            tr("<p>The <b>Books</b> example shows how to use Qt SQL classes "
               "with a model/view framework."));
}

void BookWindow::saveJobGraph(QString path)
{
    QString fileName = path + "JobGraph.gv";

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream outStream(&file);

        auto UpKeys_list = jobGraphToUp.keys();
        auto DownKeys_list = jobGraphToDown.uniqueKeys();

        QString oneString;
        oneString =  "digraph JOB_HIERARHY {\nrankdir=\"LR\"\n";
        for (auto n=0;n<UpKeys_list.size();n++)
        {
            oneString += UpKeys_list.at(n) + "[label=\"" + UpKeys_list.at(n) + "\"]\n";
        }

        for (auto n_iter=jobGraphToUp.begin();n_iter!=jobGraphToUp.end();n_iter++)
        {
            oneString += n_iter.key() + " -> " + n_iter.value() + "[style=solid]\n";
        }

        for (auto n_iter=jobGraphToDown.begin();n_iter!=jobGraphToDown.end();n_iter++)
        {
            oneString += n_iter.key() + " -> " + n_iter.value() + "[style=dashed]\n";
        }

        oneString +=  "}";

        outStream << oneString;
    }
}

QStringList BookWindow::getNeighboursJobPlaces(QString humanIdStr)
{
    QStringList places;
    QString chiefStrId = jobGraphToUp.value(humanIdStr);
    int chiefId = chiefStrId.toInt();
    if(chiefId > OtdelyBeginIndex && chiefId < LibraryBeginIndex)//мы - группа
    {
        QString otdelenieStrId = jobGraphToUp.value(chiefStrId);
        places = jobGraphToDown.values(otdelenieStrId);//типа здесь только архивы и проч.
    }
    return places;

}
