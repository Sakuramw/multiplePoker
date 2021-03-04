#include "texas.h"
#include "ui_texas.h"
#include <QTime>
#include <ctime>
#include <cstdlib>

Texas::Texas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Texas)
{
    ui->setupUi(this);
    qsrand(time(0));
//    for(int i=1;i<=13;i++){
//        if(i == 1){
//            poker<<"<font color = red>♥A</font>";
//        }else if(i == 11){
//            poker<<"<font color = red>♥J</font>";
//        }else if(i == 12){
//            poker<<"<font color = red>♥Q</font>";
//        }else if(i == 13){
//            poker<<"<font color = red>♥K</font>";
//        }else{
//            poker<<QString("<font color = red>♥%1</font>").arg(QString::number(i));
//        }
//    }
//    for(int i=1;i<=13;i++){
//        if(i == 1){
//            poker<<"<font color = red>♦A</font>";
//        }else if(i == 11){
//            poker<<"<font color = red>♦J</font>";
//        }else if(i == 12){
//            poker<<"<font color = red>♦Q</font>";
//        }else if(i == 13){
//            poker<<"<font color = red>♦K</font>";
//        }else{
//            poker<<QString("<font color = red>♦%1</font>").arg(QString::number(i));

//        }
//    }
    for(int i=1;i<=13;i++){
        if(i == 1){
            poker<<"♥A";
        }else if(i == 11){
            poker<<"♥J";
        }else if(i == 12){
            poker<<"♥Q";
        }else if(i == 13){
            poker<<"♥K";
        }else{
            poker<<QString("♥%1").arg(QString::number(i));
        }
    }
    for(int i=1;i<=13;i++){
        if(i == 1){
            poker<<"♦A";
        }else if(i == 11){
            poker<<"♦J";
        }else if(i == 12){
            poker<<"♦Q";
        }else if(i == 13){
            poker<<"♦K";
        }else{
            poker<<QString("♦%1").arg(QString::number(i));

        }
    }
    for(int i=1;i<=13;i++){
        if(i == 1){
            poker<<"♠A";
        }else if(i == 11){
            poker<<"♠J";
        }else if(i == 12){
            poker<<"♠Q";
        }else if(i == 13){
            poker<<"♠K";
        }else{
            poker<<QString("♠%1").arg(QString::number(i));
        }

    }
    for(int i=1;i<=13;i++){
        if(i == 1){
            poker<<"♣A";
        }else if(i == 11){
            poker<<"♣J";
        }else if(i == 12){
            poker<<"♣Q";
        }else if(i == 13){
            poker<<"♣K";
        }else{
            poker<<QString("♣%1").arg(QString::number(i));
        }

    }

//    shuffle();
    for(int i =0;i<7;i++){
        ui->textBrowser_log->append(poker[i]);
    }

}

Texas::~Texas()
{
    delete ui;
}

void Texas::shuffle()
{
    int x,y;
    QString temp;
    for(int i=52;i>=1;i--){
        x = qrand()%i;
        y=i-1;
        temp = poker[y];
        poker[y]=poker[x];
        poker[x]=temp;
    }
    m_sleep(17);
}

void Texas::m_sleep(int msec)
{
    QTime t1 = QTime::currentTime();
    QTime now;
    do{
        now=QTime::currentTime();
    }while(t1.msecsTo(now)<=msec);
}

void Texas::on_pushButton_clicked()
{
    ui->textBrowser_log->clear();
    shuffle();
    for(int i =0;i<7;i++){
        ui->textBrowser_log->append(poker[i]);
    }
}

void Texas::on_pushButton_cal_clicked()
{
    for(int i = 0;i<3;i++){
        for(int j = i+1;j<4;j++){
            for(int k = j+1;k<5;k++){
                for(int l = k+1;l<6;l++){
                    for(int m = l+1;m<7;m++){
                        QStringList *calPoker = new QStringList;
                        calPoker->append(poker[i]);
                        calPoker->append(poker[j]);
                        calPoker->append(poker[k]);
                        calPoker->append(poker[l]);
                        calPoker->append(poker[m]);
                        allPossible.append(calPoker);
                    }
                }
            }
        }
    }
    //单种可能性
    bool isEqualSuit = false;
    int h = 0,s = 0,c = 0,d = 0;
    for(int i =0;i<5;i++){
        if((*(allPossible[0])).at(i).contains('A')){
            sortInt.append(14);
        }else if((*(allPossible[0])).at(i).contains('J')){
            sortInt.append(11);
        }else if((*(allPossible[0])).at(i).contains('Q')){
            sortInt.append(12);
        }else if((*(allPossible[0])).at(i).contains('K')){
            sortInt.append(13);
        }else{
            sortInt.append((*(allPossible[0])).at(i).at(1).digitValue());
        }



        //同花判定
        if((*(allPossible[0])).at(i).contains("♥")){
            ++h;
        }else if((*(allPossible[0])).at(i).contains("♦")){
            ++d;
        }else if((*(allPossible[0])).at(i).contains("♠")){
            ++s;
        }else if((*(allPossible[0])).at(i).contains("♣")){
            ++c;
        }
        if(h == 5||d ==5||s==5||c==5){
            isEqualSuit = true;
            ui->textBrowser_log->append("同花");
        }
    }

}
