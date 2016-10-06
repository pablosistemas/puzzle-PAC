#ifndef INTERFACEDOJOGO_H
#define INTERFACEDOJOGO_H

#include <QWidget>
#include <QSignalMapper>
#include <QPushButton>
#include <QMessageBox>
#include <QSharedPointer>
#include <QTimer>
#include <QFile>

#include "jogo.h"

namespace Ui {
class InterfaceDoJogo;
}

class InterfaceDoJogo : public QWidget
{
    Q_OBJECT

public:
    explicit InterfaceDoJogo(QWidget *parent = 0);
    ~InterfaceDoJogo();

private:
    Ui::InterfaceDoJogo *ui;

    QSignalMapper *signalMapper;
    // TODO: pecas do tabuleiro
    QVector<QSharedPointer<QPushButton>> Pecas;

    // Classe manipula situação do posicionamento das peças no jogo
    Jogo *jogo;

    QTimer *timer;

    void updatePecas(QStringList);
    void recordFile(QTime, int);
    void showHallOfTimes(QStringList);
    void showHallOfMovements(QStringList);
    void setTextToPiece(QPushButton *, QString);

    // Shows message of victory
    QMessageBox victoryDialog;


public slots:
    // methods
    void puzzleMovement(int);
    void createNewGame();
    void updateTimer();
    void showHallOfFame();
};

#endif // INTERFACEDOJOGO_H
