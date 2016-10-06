#include "interfacedojogo.h"
#include "ui_interfacedojogo.h"

#include <QTextStream>
#include <QDir>
#include <QStringListModel>
#include <QListView>
#include <QDebug>
#include <QListWidget>
#include <QtAlgorithms>

InterfaceDoJogo::InterfaceDoJogo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InterfaceDoJogo),
    victoryDialog(this)
{
    ui->setupUi(this);

    // initializes the pieces
    jogo = new Jogo(this);

    // initializes the piece's texts randomly
    QStringList initText = jogo->getMatrizPosicoes();

    // maps the push buttons to int values corresponding the piece position number
    signalMapper = new QSignalMapper(this);

    // initializes Pecas
    // schedules objects to be deleted after control return for event loop

    Pecas.append(QSharedPointer<QPushButton>(ui->peca1, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca2, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca3, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca4, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca5, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca6, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca7, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca8, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca9, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca10, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca11, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca12, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca13, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca14, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca15, &QObject::deleteLater));
    Pecas.append(QSharedPointer<QPushButton>(ui->peca16, &QObject::deleteLater));

    updatePecas(initText);

    for(int i = 0; i < 16; ++i){
        // schedules objects to be deleted after control return for event loop
        connect(Pecas[i].data(), SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(Pecas[i].data(), i);
    }
    // connects event related to pecas
    connect(signalMapper, SIGNAL(mapped(int)),
            this, SLOT(puzzleMovement(int)));

    // connects event related to Novo Jogo
    connect(ui->newGame, SIGNAL(clicked()),
            this, SLOT(createNewGame()));

    // connects event of hall of fame
    connect(ui->hallOfFame, SIGNAL(clicked()),
            this, SLOT(showHallOfFame()));


    // defines the timer for the game
    ui->gameTimer->setDigitCount(8);
    ui->gameTimer->setSegmentStyle(QLCDNumber::Filled);

    timer = new QTimer(this);
    timer->start(1000);
    // connects the chronometer event
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));

}

void InterfaceDoJogo::puzzleMovement(int idx){

    bool isVictory = jogo->puzzleMovement(idx);
    updatePecas(jogo->getMatrizPosicoes());
    // when victory, shows dialog message and record in the file
    // the time and num of movements spent
    if(isVictory)
    {
        QTime tempoVitoria = jogo->getTime();
        int numMovimentos = jogo->getNumMovements();
        QString msg("Você ganhou!\nNúmero de jogadas: "+ QString::number(numMovimentos)+
                    "\nTempo de jogo: "+tempoVitoria.toString());
        victoryDialog.setText(msg);
        victoryDialog.exec();

        // records in File
        recordFile(tempoVitoria,numMovimentos);
    }

}

void InterfaceDoJogo::createNewGame() {
    jogo->createNewGame();
    updatePecas(jogo->getMatrizPosicoes());
    // TODO: updates chronometer

}

void InterfaceDoJogo::updateTimer() {
    // TODO: updates chronometer
    QTime t = jogo->getTime().addSecs(1);
    QString text = t.toString("hh:mm:ss");
    jogo->setTime(t);

    ui->gameTimer->display(text);
}

void InterfaceDoJogo::setTextToPiece(QPushButton *ptr, QString t){
    if(t == QString::number(16))
        ptr->setVisible(false);
    else
        ptr->setVisible(true);

    ptr->setText(t);
}

void InterfaceDoJogo::updatePecas(QStringList ql){
    setTextToPiece(ui->peca1,ql[0]);
    setTextToPiece(ui->peca2,ql[1]);
    setTextToPiece(ui->peca3,ql[2]);
    setTextToPiece(ui->peca4,ql[3]);
    setTextToPiece(ui->peca5,ql[4]);
    setTextToPiece(ui->peca6,ql[5]);
    setTextToPiece(ui->peca7,ql[6]);
    setTextToPiece(ui->peca8,ql[7]);
    setTextToPiece(ui->peca9,ql[8]);
    setTextToPiece(ui->peca10,ql[9]);
    setTextToPiece(ui->peca11,ql[10]);
    setTextToPiece(ui->peca12,ql[11]);
    setTextToPiece(ui->peca13,ql[12]);
    setTextToPiece(ui->peca14,ql[13]);
    setTextToPiece(ui->peca15,ql[14]);
    setTextToPiece(ui->peca16,ql[15]);
}

InterfaceDoJogo::~InterfaceDoJogo()
{
    delete ui;
    delete jogo;
    delete signalMapper;
}

void InterfaceDoJogo::recordFile(QTime tempo, int movimentos) {
    // enables hall of fame pushbutton if there is at least one record in file
    if(!ui->hallOfFame->isEnabled()){
        ui->hallOfFame->setEnabled(true);
    }

    QFile output;
    QDir::setCurrent("/home/bob/Documents/pablo/ufmg/sem11/pac/Puzzle/");

    output.setFileName("movimentos.txt");
    output.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out1(&output);
    out1 << movimentos << endl;
    output.close();

    output.setFileName("tempos.txt");
    output.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out2(&output);
    out2 << tempo.toString() << endl;
    output.close();
}

void InterfaceDoJogo::showHallOfFame(){
    QStringList rankingOfTimes;
    QStringList rankingOfMovements;

    QFile input;
    QDir::setCurrent("/home/bob/Documents/pablo/ufmg/sem11/pac/Puzzle/");

    input.setFileName("movimentos.txt");
    input.open(QIODevice::ReadOnly);
    QTextStream stream(&input);
    QString line;
    //int move;

    qDebug() << "teste\n";

    line = stream.readLine();
    while(!line.isNull()){

        //move = line.toInt(&isOk);
        //if(isOk)
        rankingOfMovements.append(line);
        line = stream.readLine();
    }
    input.close();

    input.setFileName("tempos.txt");
    input.open(QIODevice::ReadOnly);
    stream.setDevice(&input);

    line = stream.readLine();
    while(!line.isNull()){
        line = stream.readLine();
        //time.fromString(line);
        rankingOfTimes.append(line);
    }
    input.close();

    showHallOfMovements(rankingOfMovements);
    showHallOfTimes(rankingOfTimes);

}

void InterfaceDoJogo::showHallOfTimes(QStringList times){
    // TODO: optimize and sort and delete
    QListView *listOfMoves = new QListView();
    QStringListModel *tempos = new QStringListModel(times);
    listOfMoves->setModel(tempos);
    listOfMoves->show();
}

void InterfaceDoJogo::showHallOfMovements(QStringList moves){
    // TODO: optimize and sort and delete
    QListView *listOfMoves = new QListView();
    QStringListModel *tempos = new QStringListModel(moves);
    listOfMoves->setModel(tempos);
    listOfMoves->show();
}
