#ifndef DATATYPE_H
#define DATATYPE_H

#include <QObject>
#include <QStringList>

#include <map>

//!1�� ���ݿ��ļ��洢�ṹ
//! ʹ��map�������洢ÿ���ֶ�key��Ӧ�Ľṹ��
//! ÿ��ָ�������ֵ�������Сֵ���ֶ�������ʹ�ù�ʽ[1 | 2]
struct FieldProp{
	float sMax;
	float sMin;
	QString sLngDes;
	int sMethod;
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

	float GetAQXSthreshold();
	float GetFXthreshold();

	void SetAQXSthreshold(float value);
	void SetFXthreshold(float value);

	//! ���ݴ����һ��ֵ�ͱ�ͷ������������ۼӷ���ֵ
	float CalcFXvalue(QStringList headers, QStringList curLine);

	//! ���ݳ���ķ���ֵ�Ͱ�ȫϵ��ֵ��������������յȼ�ֵ
	int ClacFXLEVELvalue(float fxValue, float aqxsValue);
private:
	FieldMap m_fields;

	//! ��ֵ��Ϣ
	float m_aqxs_threshold;
	float m_fx_threshold;
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
