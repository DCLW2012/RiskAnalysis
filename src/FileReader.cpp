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
					FieldProp fProp = {curValue, curValue, headers[i], 1};

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
	//! ����database�����ֶ��еļ��㷽���Լ���Ա�����е���ֵ

	return true;
}

bool FileReader::ReadInputForOut(QString fileName, QString outfileName)
{
	//! ������ȡд��
	//! ���ļ���ȡ
	//��ȡ��չ����������չ����ȡ��ͬ�ĸ�ʽ
	QString ext = QFileInfo(fileName).suffix().toUpper();

	//***************�����ļ���***************************************
	//�����ļ�����
	QFile inFile(fileName);
	if(!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		inFile.close();
		return false;
	}
	QTextStream instream(&inFile);

	//***************����ļ���*************************************************
	//д������
	QFile outfile(outfileName);
	if (!outfile.open(QFile::WriteOnly | QFile::Truncate))
	{
		return false;
	}
	QTextStream outstream(&outfile);
	//���ø�ʽ����
	outstream.setRealNumberNotation(QTextStream::FixedNotation);

	//***************�߶��߽���*************************************************
	//! ��һ�У�����һ�У��������һ��
	//! ��ͷ��һ�н���
	QString headerLine = instream.readLine().toUpper();
	QStringList headers = headerLine.split(",",QString::SkipEmptyParts); //�ָ��ȡ���ļ�ͷ
	int headerNum = headers.size();

	//! �����ͷ
	QString outheaderLine = QString("%1,%2,%3").arg(headerLine).arg("OUT_FX").arg("OUT_LEVEL");
	outstream << headerLine << "\n";

	bool status = true;
	long count = 0;  
	while(status)
	{
		QString nextLine = instream.readLine().toUpper();
		
		//����һ����ǰ����б���
		QString outline;

		if (!nextLine.isEmpty())
		{
			QStringList parts = nextLine.split(",",QString::SkipEmptyParts); //�ָ��ȡ����

			//! �������ͷ�������ȣ���continue
			if(headerNum != parts.size())
			{
				count++;
				continue;
			}

			//! ����ÿһ�����,Ϊ�˺��ʹ�ð�ȫϵ��ֵ���м��㣬�ݴ��°�ȫϵ��ֵ
			float aqxsValue = 0;
			for (long i = 0; i != parts.size(); ++i)
			{
				if(headers[i].toUpper() == "AQXS")
				{
					aqxsValue = parts[i].toFloat();
				}
				outline.append(QString("%1").arg(parts[i]));
				outline.append(","); //��ӷָ���
			}

			//! ׷���������ֵ�ͷ��յȼ�
			float fxValue = m_database->CalcFXvalue(headers, parts);
			int fxLevel = m_database->ClacFXLEVELvalue(fxValue, aqxsValue);
			outline.append(QString("%1").arg(fxValue));
			outline.append(","); //��ӷָ���
			outline.append(QString("%1").arg(fxLevel));

			outstream << outline << "\n";
			status =  true;
			count++;
		}
		else //�Ƿ����
		{
			status = false;
			break;
		}
	}

	inFile.close();
	outfile.close();
	return true;
}