#ifndef DATATYPE_H
#define DATATYPE_H

#include <QObject>

//! ���ݿ��ļ��洢�ṹ
class Database : public QObject
{
	Q_OBJECT

public:
	Database();
	~Database();

private:
	
};

//2�� ���Ʋ����洢�ṹ
class DatabaseParams : public QObject
{
	Q_OBJECT

public:
	DatabaseParams();
	~DatabaseParams();

private:
	
};

//! 3���������ݲ����ṹ


#endif // DATATYPE_H
