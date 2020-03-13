#ifndef DATATYPE_H
#define DATATYPE_H

#include <QObject>

#include <map>

//!1�� ���ݿ��ļ��洢�ṹ
//! ʹ��map�������洢ÿ���ֶ�key��Ӧ�Ľṹ��
//! ÿ��ָ�������ֵ�������Сֵ
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

	//! ��ȡ�ֶθ���
	int GetFieldNum();

	FieldMap& GetFields();
private:
	FieldMap m_fields;
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
