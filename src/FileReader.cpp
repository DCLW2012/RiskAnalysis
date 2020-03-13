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
	FieldMap& fieldMap = m_database->GetFields();

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
	QFile tempFile(fileName);
	if(!tempFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QTextStream stream(&tempFile);
	//! 读取一行数据,分割数据
	QString line = stream.readLine().toUpper();
	while(!stream.atEnd())
	{
		//判断文件头标识符
		if(line.startsWith("PARAMS_METHOD"))
		{
			QStringList paramsheaders =  line.split(QRegExp(",|\\s+"), QString::SkipEmptyParts);
			if(paramsheaders.size() < 2)
			{
				continue;
			}

			//！ 判断需要读取下边的行数
			int validRow = paramsheaders[1].toInt();
			line = stream.readLine().toUpper();
			QStringList fieldNames = line.split(QRegExp(",|\\s+"), QString::SkipEmptyParts);
			line = stream.readLine().toUpper();
			QStringList methodList = line.split(QRegExp(",|\\s+"), QString::SkipEmptyParts);

			if(fieldNames.size() > methodList.size())
			{
				continue;
			}

			//! 更新到数据库变量中
			for(int i = 0; i < fieldNames.size(); ++i)
			{
				m_database->GetFields()[fieldNames[i]].sMethod = methodList[i].toInt();
			}
		}
		else if (line.startsWith("PARAMS_THRESHOLD")) //连接关系
		{
			QStringList paramsheaders =  line.split(QRegExp(",|\\s+"), QString::SkipEmptyParts);
			if(paramsheaders.size() < 2)
			{
				continue;
			}

			//！ 判断需要读取下边的行数
			int validRow = paramsheaders[1].toInt();
			for(int i = 0; i < validRow; ++ i)
			{
				line = stream.readLine().toUpper();
				QStringList list = line.split(QRegExp(",|\\s+"), QString::SkipEmptyParts);

				if("AQXS_THRESHOLD" == list[0].toUpper().trimmed())
				{
					m_database->SetAQXSthreshold(list[1].toFloat());
				}

				if("FX_THRESHOLD" == list[0].toUpper().trimmed())
				{
					m_database->SetFXthreshold(list[1].toFloat());
				}
			}
		}
		else if (line.startsWith("PARAMS_LNG")) //字段对应的文本描述
		{
			QStringList paramsheaders =  line.split(QRegExp(",|\\s+"), QString::SkipEmptyParts);
			if(paramsheaders.size() < 2)
			{
				continue;
			}

			//！ 判断需要读取下边的行数
			int validRow = paramsheaders[1].toInt();
			line = stream.readLine().toUpper();
			QStringList fieldNames = line.split(QRegExp(",|\\s+"), QString::SkipEmptyParts);
			line = stream.readLine().toUpper();
			QStringList desList = line.split(QRegExp(",|\\s+"), QString::SkipEmptyParts);

			if(fieldNames.size() > desList.size())
			{
				continue;
			}

			//! 更新到数据库变量中
			for(int i = 0; i < fieldNames.size(); ++i)
			{
				m_database->GetFields()[fieldNames[i]].sLngDes = desList[i];
			}
		}
		else
		{
			line = stream.readLine().toUpper();
		}
	}

	tempFile.close();

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
	//! 输出的表头，采用指定的lng
	QString lngDesLine;
	lngDesLine.append("ID");
	lngDesLine.append(","); //添加分隔符
	for(int i = 1; i < headers.size(); ++i)
	{
		lngDesLine.append(m_database->GetFields()[headers[i]].sLngDes);
		lngDesLine.append(","); //添加分隔符
	}
	
	QString outheaderLine = QString("%1%2,%3").arg(lngDesLine).arg(m_database->GetFields()["OUT_FX"].sLngDes).arg(m_database->GetFields()["OUT_LEVEL"].sLngDes);
	outstream << outheaderLine << "\n";

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