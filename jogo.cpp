#include "jogo.h"
#include <QtGlobal> // for qsrand() and qrand()
#include <QDebug>
#include <QPushButton>
#include <QSharedPointer>

Jogo::Jogo(QObject *parent) : QObject(parent), numMovimentos(0) {
    matrizDePosicoes = randomInitTexts();
    tempoJogo.setHMS(0,0,0);
}

// function prototype
bool randomLessThan(QString, QString);

QStringList Jogo::randomInitTexts() const {
    QStringList pos;
    /*for(int i=1; i <= 16; i++)
        pos.append(QString::number(i));

    qSort(pos.begin(), pos.end(), randomLessThan);*/

    pos.append(QString::number(1)); pos.append(QString::number(2)); pos.append(QString::number(3)); pos.append(QString::number(4)); pos.append(QString::number(5));
    pos.append(QString::number(6)); pos.append(QString::number(7)); pos.append(QString::number(8)); pos.append(QString::number(9)); pos.append(QString::number(10));
    pos.append(QString::number(11)); pos.append(QString::number(12)); pos.append(QString::number(13)); pos.append(QString::number(14)); pos.append(QString::number(16)); pos.append(QString::number(15));

    return pos;
}

QStringList Jogo::getMatrizPosicoes() const {
    return matrizDePosicoes;
}

bool randomLessThan(QString s1, QString s2)
{
    qsrand(qrand());
    return (bool)(qrand()%2); // returns 0 or 1
}

bool Jogo::puzzleMovement(int indexOfText){

    //qDebug("Clicado em %d: ", indexOfText);

    int indexOfNull;

    //QVector<QSharedPointer<QPushButton>>::iterator it;

    //QString text(Pecas[indexOfText].data()->text());
    QString text(matrizDePosicoes[indexOfText]);

    // TODO: optimize
    // looks for position of null (Peca == 16) (Make it do not visible!)
    for(indexOfNull = 0; matrizDePosicoes[indexOfNull] != QString("16"); indexOfNull++);

    // changes places whether null place is lower neighbor of text place
    if((indexOfNull%4)-(indexOfText%4) == 0 && (indexOfNull/4)-(indexOfText/4) == 1){
        this->swapPecas(indexOfText,indexOfNull);
        numMovimentos++;
    }
    // changes places whether null place is upper neighbor of text place
    else if((indexOfNull%4)-(indexOfText%4) == 0 && (indexOfText/4)-(indexOfNull/4) == 1){
        this->swapPecas(indexOfText,indexOfNull);
        numMovimentos++;
    }
    // changes places whether null place is left neighbor of text place:
    // distance of 1 and same line comparison
    else if(indexOfNull-indexOfText == 1 && (indexOfText/4)-(indexOfNull/4) == 0){
        this->swapPecas(indexOfText,indexOfNull);
        numMovimentos++;
    }
    // changes places whether null place is right neighbor of text place
    else if(indexOfText-indexOfNull == 1 && (indexOfText/4)-(indexOfNull/4) == 0){
        this->swapPecas(indexOfText,indexOfNull);
        numMovimentos++;
    }
    // else do nothing!

    return testVictory();


    // TODO: emit signal of victory!
}

void Jogo::createNewGame() {
    // clean num movements
    numMovimentos = 0;
    matrizDePosicoes = randomInitTexts();

    //TODO: clean chronometer
    tempoJogo.setHMS(0,0,0);
}

QTime Jogo::getTime() const {
    return tempoJogo;
}

void Jogo::setTime(QTime t){
    tempoJogo.setHMS(t.hour(),t.minute(),t.second());
}

void Jogo::swapPecas(int idx1, int idx2){
    QString temp(matrizDePosicoes[idx1]);
    matrizDePosicoes[idx1] = matrizDePosicoes[idx2];
    matrizDePosicoes[idx2] = temp;
}

bool Jogo::testVictory() const {
    int i;
    for(i = 0; i < 16; i++){
        if(matrizDePosicoes[i] != QString::number(i+1)){
            break;
        }
    }
    return (bool)(i == 16);
}

int Jogo::getNumMovements() const {
    return numMovimentos;
}
