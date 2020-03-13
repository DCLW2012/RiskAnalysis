#ifndef DATATYPE_H
#define DATATYPE_H

#include <QObject>

//! 数据库文件存储结构
class Database : public QObject
{
	Q_OBJECT

public:
	Database();
	~Database();

private:
	
};

//2、 控制参数存储结构
class DatabaseParams : public QObject
{
	Q_OBJECT

public:
	DatabaseParams();
	~DatabaseParams();

private:
	
};

//! 3、输入数据参数结构


#endif // DATATYPE_H
