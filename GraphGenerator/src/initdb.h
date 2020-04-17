/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
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

#ifndef INITDB_H
#define INITDB_H

#include <QtSql>
#include <QVector>
#include "bookwindow.h"

long long InstitutyBeginIndex = 150002;//150001 - кабинет директора
long long InstitutyNum = 200;
//int InstitutyProb = 1;

long long OtdeleniyaBeginIndex = 151000;
long long OtdeleniyaNum = 500;
//int OtdeleniyaProb = 10;

long long OtdelyBeginIndex = 152000;
long long OtdelyNum = 900;
//int OtdelyProb = 90;

long long  LibraryBeginIndex = 153000;
long long LibraryNum = 900;
//int LibraryProb = 107;

int GroupBeginIndex = 155000;
//int GroupNum = 4000;
//int GroupProb = 3500;

int ShopBeginIndex = 130051;
//int ShopNum = 25;//4/9/16/25/36/49...
//int ShopProb = 3550;

void addRecord(QSqlQuery &q, int human_id, int home_id, int age, const QString &job_sites, const QString &holiday_sites)
{
    q.addBindValue(human_id);
    q.addBindValue(home_id);
    q.addBindValue(age);
    q.addBindValue(job_sites);
    q.addBindValue(holiday_sites);
    q.exec();
}

QVariant addGenre(QSqlQuery &q, const QString &name)
{
    q.addBindValue(name);
    q.exec();
    return q.lastInsertId();
}

QVariant addAuthor(QSqlQuery &q, const QString &name, QDate birthdate)
{
    q.addBindValue(name);
    q.addBindValue(birthdate);
    q.exec();
    return q.lastInsertId();
}

const auto HUMANS_SQL = QLatin1String(R"(
    create table humans(id integer primary key, human integer, home integer, age integer,
                       job_sites varchar, holiday_sites varchar)
    )");

const auto HUMANS_DEL_SQL = QLatin1String(R"(drop table humans)"
            );

const auto HUMANSLOAD_SQL = QLatin1String(R"(
    create table humansload(id integer primary key, human integer, home integer, age integer,
                       job_sites varchar, holiday_sites varchar)
    )");

const auto AUTHORS_SQL =  QLatin1String(R"(
    create table authors(id integer primary key, name varchar, birthdate date)
    )");

const auto GENRES_SQL = QLatin1String(R"(
    create table genres(id integer primary key, name varchar)
    )");

const auto INSERT_AUTHOR_SQL = QLatin1String(R"(
    insert into authors(name, birthdate) values(?, ?)
    )");

const auto INSERT_HUMAN_SQL = QLatin1String(R"(
    insert into humans(human, home, age, job_sites, holiday_sites)
                      values(?, ?, ?, ?, ?)
    )");

const auto INSERT_GENRE_SQL = QLatin1String(R"(
    insert into genres(name) values(?)
    )");

QString BookWindow::getHolidayPlaces(int row, int col)
{
    QString places;
    int borderWidth = 5;
    double d_shop_dist = sqrt(ui.ShopNum->text().toInt());
    int shop_dist = d_shop_dist;
    int radiusForOneShop = 158/shop_dist;
    for (int s=0;s<shop_dist;s++)
    {
        int s_row = radiusForOneShop*s + radiusForOneShop/2;
        for (int f=0;f<shop_dist;f++)
        {
            int f_row = radiusForOneShop*f + radiusForOneShop/2;
            if(sqrt((row-s_row)*(row-s_row) + (col-f_row)*(col-f_row)) < radiusForOneShop + borderWidth)
            {
                int shopNum = ShopBeginIndex + f+shop_dist*s;
                places.append(QString::number(shopNum) + ";");
            }
        }
    }    
    return places;
}

QSqlError BookWindow::initDb(int generSeed)
{
   /*QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");

    if (!db.open())
        return db.lastError();*/

    QSqlDatabase db = QSqlDatabase::database();//дефолтное соединение

    QSqlQuery q;
    QStringList tables = db.tables();
    if (tables.contains("humans", Qt::CaseInsensitive))
        q.exec(HUMANS_DEL_SQL); //return QSqlError();

    if (!q.exec(HUMANS_SQL))
        return q.lastError();

    //if (!q.exec(AUTHORS_SQL))
    //    return q.lastError();
    //if (!q.exec(GENRES_SQL))
    //    return q.lastError();

    /*if (!q.prepare(INSERT_AUTHOR_SQL))
        return q.lastError();
    QVariant asimovId = addAuthor(q, QLatin1String("Isaac Asimov"), QDate(1920, 2, 1));
    QVariant greeneId = addAuthor(q, QLatin1String("Graham Greene"), QDate(1904, 10, 2));
    QVariant pratchettId = addAuthor(q, QLatin1String("Terry Pratchett"), QDate(1948, 4, 28));

    if (!q.prepare(INSERT_GENRE_SQL))
        return q.lastError();
    QVariant sfiction = addGenre(q, QLatin1String("Science Fiction"));
    QVariant fiction = addGenre(q, QLatin1String("Fiction"));
    QVariant fantasy = addGenre(q, QLatin1String("Fantasy"));
    */
    if (!q.prepare(INSERT_HUMAN_SQL))
        return q.lastError();

    int current_home;
    int current_age;
    QString current_job;
    QString current_holiday;
    QRandomGenerator rg = QRandomGenerator();
    rg.seed(generSeed);

    //long long beginIndex = 150002;//150001 - кабинет директора
    QString index;

    QVector<QString> institutyLocVec;
    for (long long i=0;i<InstitutyNum;i++)
    {
        index = QString::number(InstitutyBeginIndex + i);
        institutyLocVec.push_back(index);
    }
    int institutyLocIndex = 0;

    //beginIndex = 151000;
    QVector<QString> otdeleniyaLocVec;
    for (long long i=0;i<OtdeleniyaNum;i++)
    {
        index = QString::number(OtdeleniyaBeginIndex + i);
        otdeleniyaLocVec.push_back(index);
    }
    int otdeleniyaLocIndex = 0;

    //beginIndex = 152000;
    QVector<QString> otdelyLocVec;
    for (long long i=0;i<OtdelyNum;i++)
    {
        index = QString::number(OtdelyBeginIndex + i);
        otdelyLocVec.push_back(index);
    }
    int otdelyLocIndex = 0;

    //beginIndex = 153000;
    QVector<QString> libraryLocVec;
    for (long long i=0;i<LibraryNum;i++)
    {
        index = QString::number(LibraryBeginIndex + i);
        libraryLocVec.push_back(index);
    }
    int libraryLocIndex = 0;

    QHash<QString,bool> groups;
    QHash<QString,bool> shops;
    int employers = 0;
    int saylers = 0;
    bool directorSet = false;
    for (int j=0;j<316;j++)
    {
        for (int i=0;i<316;i++)
        {
            int cur_human = 316*j + i;
            current_home = 100000 + cur_human/4;
            current_holiday = getHolidayPlaces(i/2,j/2);
            current_age = rg.bounded(85);
            if(!directorSet && current_age == 72)
            {
                current_job = QString::number(150001);
                directorSet = true;
            }
            else if(current_age > 18 && current_age < 65)
            {
                int occup = rg.bounded(0,10000);
                if(occup <= ui.InstitutyProb->text().toInt())
                    current_job = institutyLocVec.at(institutyLocIndex++);//нач. институтов и департаментов
                else if(occup <= ui.OtdeleniyaProb->text().toInt())
                    current_job = otdeleniyaLocVec.at(otdeleniyaLocIndex++);//нач.отделений
                else if( occup <= ui.OtdelyProb->text().toInt())
                    current_job = otdelyLocVec.at(otdelyLocIndex++);//нач.отделов
                else if(occup <= ui.LibraryProb->text().toInt())
                    current_job = libraryLocVec.at(libraryLocIndex++);//библиотеки и пр.
                else if(occup <= ui.GroupProb->text().toInt())
                {
                    current_job = QString::number(rg.bounded(GroupBeginIndex,GroupBeginIndex+ui.GroupNum->text().toInt()));//остальные сотрудники
                    if(groups.find(current_job) == groups.end())
                        groups.insert(current_job,true);
                    employers++;
                }
                else if(occup <= ui.ShopProb->text().toInt())
                {
                    current_job = QString::number(rg.bounded(ShopBeginIndex,ShopBeginIndex+ui.ShopNum->text().toInt()));//продавцы
                    if(shops.find(current_job) == shops.end())
                        shops.insert(current_job,true);
                    saylers++;
                }
                else
                    current_job.clear();
            }
            else
                current_job.clear();
           addRecord(q, cur_human, current_home, current_age, current_job, current_holiday);
        }
    }


    int groupIdx = groups.size();
    int shopsIdx = shops.size();

    ui.lineEditInstituts->setText(QString::number(institutyLocIndex));
    ui.lineEditOtdeleniys->setText(QString::number(otdeleniyaLocIndex));
    ui.lineEditOtdels->setText(QString::number(otdelyLocIndex));
    ui.lineEditLibrs->setText(QString::number(libraryLocIndex));
    ui.lineEditShops->setText(QString::number(shopsIdx));
    ui.lineEditSaylers->setText(QString::number(saylers));
    ui.lineEditEmployers->setText(QString::number(employers));
    ui.lineEditGroup->setText(QString::number(groupIdx));
    ui.lineEditEmplInGroup->setText(QString::number(employers/groupIdx));

    int numLibraryInEveryOtdelenie = libraryLocIndex/otdeleniyaLocIndex;
    int beginLibrary = 0;
    int leastGroups = groups.size();
    int numGroupsInEveryOtdel = leastGroups/otdelyLocIndex;
    QHashIterator<QString, bool> groupIterator(groups);
    groupIterator.next();
    int numOtdeleniyaInEveryInstitut = otdeleniyaLocIndex/institutyLocIndex+1;
    int beginOtdeleniya = 0;
    int beginOtdely = 0;
    int numOtdelyInEveryOtdelenie = otdelyLocIndex/otdeleniyaLocIndex+1;
    for (int k_1=0;k_1<institutyLocIndex;k_1++)
    {
        jobGraphToUp.insert(institutyLocVec.at(k_1), QString::number(150001));
        for (int k_2=beginOtdeleniya;k_2<beginOtdeleniya + numOtdeleniyaInEveryInstitut;k_2++)
        {
           if(k_2 >= otdeleniyaLocVec.size()) continue;
           jobGraphToUp.insert(otdeleniyaLocVec.at(k_2), institutyLocVec.at(k_1));
           for (int k_3=beginOtdely;k_3<beginOtdely+numOtdelyInEveryOtdelenie;k_3++)
           {
               if(k_3 >= otdelyLocVec.size()) continue;
               jobGraphToUp.insert(otdelyLocVec.at(k_3), otdeleniyaLocVec.at(k_2));
               //jobGraphToDown.insertMulti(otdeleniyaLocVec.at(k_2), otdelyLocVec.at(k_3));
               int counter = 0;               
               while(counter < numGroupsInEveryOtdel)
               {
                   jobGraphToUp.insert(groupIterator.key(), otdelyLocVec.at(k_3));
                   if (groupIterator.hasNext())
                       groupIterator.next();
                   counter++;
               }
               leastGroups -= counter;
               if(otdelyLocIndex - beginOtdely > 0)
               numGroupsInEveryOtdel = leastGroups/(otdelyLocIndex - beginOtdely);
           }
           beginOtdely += numOtdelyInEveryOtdelenie;

           //распределяем архивы и прочее по отделениям
           for (int k_5=beginLibrary;k_5<beginLibrary+numLibraryInEveryOtdelenie;k_5++)
           {
               jobGraphToUp.insert(libraryLocVec.at(k_5), otdeleniyaLocVec.at(k_2));
               jobGraphToDown.insertMulti(otdeleniyaLocVec.at(k_2), libraryLocVec.at(k_5));
           }
           beginLibrary += numLibraryInEveryOtdelenie;

        }
        beginOtdeleniya+=numOtdeleniyaInEveryInstitut;
    }

    return QSqlError();
}

#endif
