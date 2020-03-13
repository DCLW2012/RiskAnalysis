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
	//! 打开文件读取
	//获取扩展名，根据扩展名读取不同的格式
	QString ext = QFileInfo(fileName).suffix().toUpper();

	//解析文件内容
	QFile tempFile(fileName);
	if(!tempFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		tempFile.close();
		return false;
	}
	
	//! 标头第一行解析
	QTextStream stream(&tempFile);
	QString headerLine = stream.readLine().toUpper();
	QStringList headers = headerLine.split(",",QString::SkipEmptyParts); //分割读取的文件头
	int headerNum = headers.size();
	
	//! 初始化一个database对象，更新各个字段最大值最小值
	m_database = new Database();
	FieldMap fieldMap = m_database->GetFields();

	bool status = true;
	long count = 0;  
	while(status)
	{
		QString nextLine = stream.readLine().toUpper();

		if (!nextLine.isEmpty())
		{
			QStringList parts = nextLine.split(",",QString::SkipEmptyParts); //分割读取的行

			//! 个数与标头个数不等，则continue
			if(headerNum != parts.size())
			{
				count++;
				continue;
			}

			//! 记录当前最大最小值

			//! 遍历每个列的值，更新各个字段的最大最小值即可
			//! 跳过ID，第一列
			for (long i = 1; i != parts.size(); ++i)
			{
				//! 初始化每个字段的最大最小值
				float curValue = parts[i].toFloat();
				if(count == 0){
					//! 属性结构体
					FieldProp fProp = {curValue, curValue, headers[i]};

					fieldMap[headers[i] ] = fProp ;
				}else   //! 计算更新最小最大值
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
		else //是否结束
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