#include "DataType.h"

Database::Database()
	: QObject()
	, m_aqxs_threshold(2.5)
	, m_fx_threshold(25)
{

}

Database::~Database()
{

}

int Database::GetFieldNum()
{
	return m_fields.size();
}

FieldMap& Database::GetFields()
{
	return m_fields;
}

float Database::GetAQXSthreshold()
{
	return m_aqxs_threshold;
}

float Database::GetFXthreshold()
{
	return m_fx_threshold;
}

void Database::SetAQXSthreshold(float value)
{
	m_aqxs_threshold = value;
}
	
void Database::SetFXthreshold(float value)
{
	m_fx_threshold = value;
}


//! ���ݴ����һ��ֵ�ͱ�ͷ������������ۼӷ���ֵ
float Database::CalcFXvalue(QStringList headers, QStringList curLine)
{
	return 20;
}

//! ���ݳ���ķ���ֵ�Ͱ�ȫϵ��ֵ��������������յȼ�ֵ
int Database::ClacFXLEVELvalue(float fxValue, float aqxsValue)
{
	return 1;
}

//*****************************************************************
DatabaseParams::DatabaseParams()
	: QObject()
{

}

DatabaseParams::~DatabaseParams()
{

}
