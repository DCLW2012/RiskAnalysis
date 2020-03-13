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
					FieldProp fProp = {curValue, curValue, headers[i], 1};

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
	//! 更新database属性字段中的计算方法以及成员变量中的阈值

	return true;
}

bool FileReader::ReadInputForOut(QString fileName, QString outfileName)
{
	//! 遍历读取写出
	//! 打开文件读取
	//获取扩展名，根据扩展名读取不同的格式
	QString ext = QFileInfo(fileName).suffix().toUpper();

	//***************输入文件流***************************************
	//解析文件内容
	QFile inFile(fileName);
	if(!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		inFile.close();
		return false;
	}
	QTextStream instream(&inFile);

	//***************输出文件流*************************************************
	//写出数据
	QFile outfile(outfileName);
	if (!outfile.open(QFile::WriteOnly | QFile::Truncate))
	{
		return false;
	}
	QTextStream outstream(&outfile);
	//设置格式控制
	outstream.setRealNumberNotation(QTextStream::FixedNotation);

	//***************边读边解析*************************************************
	//! 读一行，计算一行，输出到流一行
	//! 标头第一行解析
	QString headerLine = instream.readLine().toUpper();
	QStringList headers = headerLine.split(",",QString::SkipEmptyParts); //分割读取的文件头
	int headerNum = headers.size();

	//! 输出表头
	QString outheaderLine = QString("%1,%2,%3").arg(headerLine).arg("OUT_FX").arg("OUT_LEVEL");
	outstream << headerLine << "\n";

	bool status = true;
	long count = 0;  
	while(status)
	{
		QString nextLine = instream.readLine().toUpper();
		
		//定义一个当前输出行变量
		QString outline;

		if (!nextLine.isEmpty())
		{
			QStringList parts = nextLine.split(",",QString::SkipEmptyParts); //分割读取的行

			//! 个数与标头个数不等，则continue
			if(headerNum != parts.size())
			{
				count++;
				continue;
			}

			//! 遍历每一行输出,为了后边使用安全系数值进行计算，暂存下安全系数值
			float aqxsValue = 0;
			for (long i = 0; i != parts.size(); ++i)
			{
				if(headers[i].toUpper() == "AQXS")
				{
					aqxsValue = parts[i].toFloat();
				}
				outline.append(QString("%1").arg(parts[i]));
				outline.append(","); //添加分隔符
			}

			//! 追加输出风险值和风险等级
			float fxValue = m_database->CalcFXvalue(headers, parts);
			int fxLevel = m_database->ClacFXLEVELvalue(fxValue, aqxsValue);
			outline.append(QString("%1").arg(fxValue));
			outline.append(","); //添加分隔符
			outline.append(QString("%1").arg(fxLevel));

			outstream << outline << "\n";
			status =  true;
			count++;
		}
		else //是否结束
		{
			status = false;
			break;
		}
	}

	inFile.close();
	outfile.close();
	return true;
}