#include <QFileInfo>
#include <QTextStream>
#include <QStringList>

#include "FileReader.h"
#include "DataType.h"

FileReader::FileReader(QObject *parent)
	: QObject(parent)
	, m_database(nullptr)
{
	
}

FileReader::~FileReader()
{

}

bool FileReader::ReadDatabase(QString fileName)
{
	//! ���ļ���ȡ
	//��ȡ��չ����������չ����ȡ��ͬ�ĸ�ʽ
	QString ext = QFileInfo(fileName).suffix().toUpper();

	//�����ļ�����
	QFile tempFile(fileName);
	if(!tempFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		tempFile.close();
		return false;
	}
	
	//! ��ͷ��һ�н���
	QTextStream stream(&tempFile);
	QString headerLine = stream.readLine().toUpper();
	QStringList headers = headerLine.split(",",QString::SkipEmptyParts); //�ָ��ȡ���ļ�ͷ
	int headerNum = headers.size();
	
	//! ��ʼ��һ��database���󣬸��¸����ֶ����ֵ��Сֵ
	m_database = new Database();
	FieldMap fieldMap = m_database->GetFields();

	bool status = true;
	long count = 0;  
	while(status)
	{
		QString nextLine = stream.readLine().toUpper();

		if (!nextLine.isEmpty())
		{
			QStringList parts = nextLine.split(",",QString::SkipEmptyParts); //�ָ��ȡ����

			//! �������ͷ�������ȣ���continue
			if(headerNum != parts.size())
			{
				count++;
				continue;
			}

			//! ��¼��ǰ�����Сֵ

			//! ����ÿ���е�ֵ�����¸����ֶε������Сֵ����
			//! ����ID����һ��
			for (long i = 1; i != parts.size(); ++i)
			{
				//! ��ʼ��ÿ���ֶε������Сֵ
				float curValue = parts[i].toFloat();
				if(count == 0){
					//! ���Խṹ��
					FieldProp fProp = {curValue, curValue, headers[i]};

					fieldMap[headers[i] ] = fProp ;
				}else   //! ���������С���ֵ
				{
					if(curValue < fieldMap[headers[i] ].sMin){
						fieldMap[headers[i] ].sMin = curValue;
					}

					if(curValue > fieldMap[headers[i] ].sMax){
						fieldMap[headers[i] ].sMax = curValue;
					}
				}
			}
			status =  true;
			count++;
		}
		else //�Ƿ����
		{
			status = false;
			break;
		}
	}

	tempFile.close();


	return true;
}


bool FileReader::ReadDatabaseParam(QString fileName)
{
	return true;
}

bool FileReader::ReadInput(QString fileName)
{
	return true;
}