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


float Database::MethodOne(float value, QString fieldName)
{
	float minValue = this->m_fields[fieldName].sMin;
	float maxValue = this->m_fields[fieldName].sMax;

	float range = maxValue - minValue;
	float disFromMin = value - minValue;

	if(range == 0)
	{
		return 0;
	}

	float result = 4 * disFromMin / range + 1;
	return result;
}

float Database::MethodTwo(float value, QString fieldName)
{
	float minValue = this->m_fields[fieldName].sMin;
	float maxValue = this->m_fields[fieldName].sMax;

	float range = maxValue - minValue;
	float disFromMax = maxValue - value;

	if(range == 0)
	{
		return 0;
	}

	float result = 4 * disFromMax / range + 1;
	return result;
}

//! ���ݴ����һ��ֵ�ͱ�ͷ������������ۼӷ���ֵ
float Database::CalcFXvalue(QStringList headers, QStringList curLine)
{
	//! ������һ��ID
	float sum =0;
	for(int i = 1; i  != headers.size(); ++i)
	{
		//! �жϷ���ֵ���ö�Ӧ�ĺ���
		float value = 0;
		if(this->m_fields[headers[i]].sMethod == 1)
		{
			value = this->MethodOne(curLine[i].toFloat(), headers[i]);
			
		}else if(this->m_fields[headers[i]].sMethod == 2)
		{
			value = this->MethodTwo(curLine[i].toFloat(), headers[i]);
		}else
		{
			value = this->MethodOne(curLine[i].toFloat(), headers[i]);
		}
		sum += value;
	}

	return sum;
}

//! ���ݳ���ķ���ֵ�Ͱ�ȫϵ��ֵ��������������յȼ�ֵ
int Database::ClacFXLEVELvalue(float fxValue, float aqxsValue)
{
	//  |      0     |      1     
	//  ----------------------------��fx�����
	//  |      2     |      3
	//(aqxs���)
	
	float fxthreshold = this->GetFXthreshold();
	float aqxsthreshold = this->GetAQXSthreshold();
	if(fxValue <= fxthreshold && aqxsValue > aqxsthreshold)
	{
		return 0;
	}

	if(fxValue > fxthreshold && aqxsValue > aqxsthreshold)
	{
		return 1;
	}

	if(fxValue > fxthreshold && aqxsValue <= aqxsthreshold)
	{
		return 3;
	}

	if(fxValue <= fxthreshold && aqxsValue <= aqxsthreshold)
	{
		return 2;
	}

	return 0;
}

//*****************************************************************
DatabaseParams::DatabaseParams()
	: QObject()
{

}

DatabaseParams::~DatabaseParams()
{

}
