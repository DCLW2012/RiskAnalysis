#ifndef DATATYPE_H
#define DATATYPE_H

#include <QObject>
#include <QStringList>

#include <map>

//!1、 数据库文件存储结构
//! 使用map容器，存储每个字段key对应的结构体
//! 每个指标的属性值，最大最小值，字段描述，使用公式[1 | 2]
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

	//! 获取字段个数
	int GetFieldNum();

	FieldMap& GetFields();

	float GetAQXSthreshold();
	float GetFXthreshold();

	void SetAQXSthreshold(float value);
	void SetFXthreshold(float value);

	//! 根据传入的一行值和表头，计算输出个累加风险值
	float CalcFXvalue(QStringList headers, QStringList curLine);

	//! 根据出入的风险值和安全系数值，计算输出个风险等级值
	int ClacFXLEVELvalue(float fxValue, float aqxsValue);
private:
	FieldMap m_fields;

	//! 阈值信息
	float m_aqxs_threshold;
	float m_fx_threshold;
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
