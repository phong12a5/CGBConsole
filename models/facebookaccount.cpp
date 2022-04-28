#include "facebookaccount.h"

#include <QDateTime>
#include "Utility.h"

QStringList FacebookAccount::m_firstNameMales = QStringList();
QStringList FacebookAccount::m_firstNameFemales = QStringList();
QStringList FacebookAccount::m_lastNames = QStringList();
PasswdGenerator FacebookAccount::m_passGenerator = PasswdGenerator();

FacebookAccount::FacebookAccount(QObject *parent,QString username,QString password,QString _2fa) :
    QObject(parent)
{
    this->username = username;
    this->password = password;
    this->_2fa = _2fa;
}

FacebookAccount::~FacebookAccount()
{

}

void FacebookAccount::setUsing(bool isUsing)
{

    this->isusing = isUsing;
}


bool FacebookAccount::isInusing()
{
    return this->isusing;
}

void FacebookAccount::setDisable(bool isDisable)
{
    this->isdisable = isDisable;
}


bool FacebookAccount::isDisable()
{
    return this->isdisable;
}

void FacebookAccount::setChecked(bool isChecked)
{
    this->ischecked = isChecked;
}

bool FacebookAccount::isChecked()
{
    return this->ischecked;
}

QString FacebookAccount::getHo() const
{
    return m_ho;
}

void FacebookAccount::setHo(const QString &ho)
{
    m_ho = ho;
}

QString FacebookAccount::getTen() const
{
    return m_ten;
}

void FacebookAccount::setTen(const QString &ten)
{
    m_ten = ten;
}

QDateTime FacebookAccount::getBirthDay() const
{
    return m_birthDay;
}

void FacebookAccount::setBirthDay(const QDateTime &birthDay)
{
    m_birthDay = birthDay;
}

Genre FacebookAccount::getGenre() const
{
    return m_genre;
}

void FacebookAccount::setGenre(const Genre &genre)
{
    m_genre = genre;
}

QString FacebookAccount::getProxy() const
{
    return m_proxy;
}

void FacebookAccount::setProxy(const QString &proxy)
{
    m_proxy = proxy;
}

QJsonObject FacebookAccount::toJsonObject()
{
    QJsonObject object;
    object.insert("username",username);
    object.insert("password",password);
    object.insert("2fa",_2fa);
    object.insert("isChangedPass",isChangedPass);
    return object;
}

QString FacebookAccount::toString()
{
    return username+"|"+password+"|"+_2fa;
//    return m_ho+" "+m_ten+"|"+m_birthDay.toString("yyyy-MM-dd");
}

QString FacebookAccount::toFullString()
{
    return username+"|"+password+"|"+m_ho+"|"+m_ten+"|"
            +QString::number(m_birthDay.daysTo(QDateTime::currentDateTime())/365)+"|"
            +(m_genre==MALE?"Nam":"Nữ")+"|"
            +_2fa;
}

QString FacebookAccount::toEncodableString()
{
    return username+"|"+password+"|"+_2fa+"|"+(isdisable?"Disable":"")+"|"+(ischecked?"Checked":"");
}

void FacebookAccount::loadData()
{
    qDebug()<<__FUNCTION__;
    if(m_firstNameMales.size() == 0){
        QString filePath = Utility::getExecutedPath()+"/data/VN/firstname_male.txt";
        QFileInfo info(filePath);
        if(info.exists()){
            QStringList list = Utility::read(filePath).split("\n");
            for(int i=0;i<list.size();i++){
                QString item = list[i].remove("\n").remove("\r");
                if(item.size()>0){
                    m_firstNameMales<<item;
                }
            }
        }

//        filePath = Utility::getExecutedPath()+"/data/US/firstname_male.txt";
//        info = QFileInfo(filePath);
//        if(info.exists()){
//            QStringList list = Utility::read(filePath).split("\n");
//            for(int i=0;i<list.size();i++){
//                QString item = list[i].remove("\n").remove("\r");
//                if(item.size()>0){
//                    m_firstNameMales<<item;
//                }
//            }
//        }
    }

    if(m_firstNameFemales.size() == 0){
        QString filePath = Utility::getExecutedPath()+"/data/VN/firstname_female.txt";
        QFileInfo info(filePath);
        if(info.exists()){
            QStringList list = Utility::read(filePath).split("\n");
            for(int i=0;i<list.size();i++){
                QString item = list[i].remove("\n").remove("\r");
                if(item.size()>0){
                    m_firstNameFemales<<item;
                }
            }
        }

//        filePath = Utility::getExecutedPath()+"/data/US/firstname_female.txt";
//        info = QFileInfo(filePath);
//        if(info.exists()){
//            QStringList list = Utility::read(filePath).split("\n");
//            for(int i=0;i<list.size();i++){
//                QString item = list[i].remove("\n").remove("\r");
//                if(item.size()>0){
//                    m_firstNameFemales<<item;
//                }
//            }
//        }
    }

    if(m_lastNames.size() == 0){
        QString filePath = Utility::getExecutedPath()+"/data/VN/lastname.txt";
        QFileInfo info(filePath);
        if(info.exists()){
            QStringList list = Utility::read(filePath).split("\n");
            for(int i=0;i<list.size();i++){
                QString item = list[i].remove("\n").remove("\r");
                if(item.size()>0){
                    m_lastNames<<item;
                }
            }
        }

        filePath = Utility::getExecutedPath()+"/data/US/lastname.txt";
        info = QFileInfo(filePath);
        if(info.exists()){
            QStringList list = Utility::read(filePath).split("\n");
            for(int i=0;i<list.size();i++){
                QString item = list[i].remove("\n").remove("\r");
                if(item.size()>0){
                    m_lastNames<<item;
                }
            }
        }
    }
}

FacebookAccount* FacebookAccount::fromJson(QJsonObject obj)
{
    FacebookAccount *acc = new FacebookAccount();
    if(obj.contains("username")){
        acc->setUsername(obj["username"].toString());
    }
    if(obj.contains("password")){
        acc->setPassword(obj["password"].toString());
    }
    if(obj.contains("2fa")){
        acc->set2fa(obj["2fa"].toString());
    }
    if(obj.contains("isChangedPass")){
        acc->setIsChangedPass(obj["isChangedPass"].toBool());
    }
    return acc;
}

void FacebookAccount::setPassword(const QString &value)
{
    password = value;
}

QString FacebookAccount::get2fa() const
{
    return _2fa;
}

void FacebookAccount::set2fa(const QString &_2fa)
{
    this->_2fa = _2fa;
}

QString FacebookAccount::getUsername() const
{
    return username;
}

void FacebookAccount::setUsername(const QString &value)
{
    username = value;
}

FacebookAccount* FacebookAccount::generate()
{
    FacebookAccount *rtAcc = new FacebookAccount();
    rtAcc->setGenre(getRandomGenre());
    rtAcc->setHo(getRandomLastName());
    rtAcc->setTen(getRandomFirstName(rtAcc->getGenre()));
    rtAcc->setBirthDay(getRandomBirthDay());
    rtAcc->setPassword(m_passGenerator.generate(8));

    qDebug()<<rtAcc->toFullString();
    return rtAcc;
}

Genre FacebookAccount::getRandomGenre()
{
    if(Utility::random()%2){
        return FEMALE;
    }
    return MALE;
}

QString FacebookAccount::getRandomLastName()
{
    if(m_lastNames.size()==0){
        loadData();
        return "";
    }
    int rd = Utility::random()%m_lastNames.size();
    return m_lastNames[rd];
}

QString FacebookAccount::getRandomFirstName(Genre genre)
{
    if(genre == MALE){
        if(m_firstNameMales.size() == 0){
            loadData();
            return "";
        }
        int rd = Utility::random()%m_firstNameMales.size();
        return m_firstNameMales[rd];
    }

    if(m_firstNameFemales.size() == 0){
        loadData();
        return "";
    }
    int rd = Utility::random()%m_firstNameFemales.size();
    return m_firstNameFemales[rd];

}

QDateTime FacebookAccount::getRandomBirthDay(int minAge)
{
    int rd = Utility::random()%((40-minAge)*31536000)+minAge*31536000;//365*24*60*60;
    return QDateTime::fromSecsSinceEpoch(QDateTime::currentSecsSinceEpoch()-rd);
}

QString FacebookAccount::getPassword() const
{
    return password;
}

bool FacebookAccount::getIsChangedPass()
{
    return isChangedPass;
}

void FacebookAccount::setIsChangedPass(bool value)
{
    isChangedPass = value;
}

