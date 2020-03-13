#ifndef DATATYPE_H
#define DATATYPE_H

#include <QObject>

#include <map>

//!1、 数据库文件存储结构
//! 使用map容器，存储每个字段key对应的结构体
//! 每个指标的属性值，最大最小值
struct FieldProp{
	float sMax;
	float sMin;
	QString sLngDes;
};

typedef std::map<QString, FieldProp> FieldMap;
class Database : public QObject
{
	Q_OBJECT

public:
	Database();
	~Database();

	//! 获取字段个数
	int GetFieldNum();

	FieldMap& GetFields();
private:
	FieldMap m_fields;
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
