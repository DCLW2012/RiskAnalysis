
#include <QtCore/QCoreApplication>
#include <QDir>
#include <QFile>
#include "QTextCodec"

#include "FileReader.h"
#include "Logger.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	//国际化
	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

	//!! 日志系统，存储到log目录下
	//! 初始化创建log文件
	Logger::CreateNewLogFile(QCoreApplication::applicationName());

	//! 当前工作路径
	QString curDirectory = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());

	QString databaseFullPath = curDirectory + "\\data\\database.txt";
	QString dbParamsFullPath = curDirectory + "\\data\\database_params.txt";
	QString inputFullPath = curDirectory + "\\data\\input.txt";
	QString outputFullPath = curDirectory + "\\data\\output.txt";
	
	//! 每次计算先删除outputFullPath对应的文件
	QFile file(outputFullPath);
	if (file.exists())
	{
		file.remove();
    }

	//! 读取文件
	FileReader fReader = new FileReader(nullptr);
	bool isdbSuccess = fReader.ReadDatabase(databaseFullPath);
	bool isdbparamSuccess = fReader.ReadDatabaseParam(dbParamsFullPath);

	//! 计算输出
	if(isdbSuccess && isdbparamSuccess)
	{
		bool isSuccess = fReader.ReadInputForOut(inputFullPath, outputFullPath);

		if(isSuccess)
		{
			a.quit();
			Logger::Message(QString("计算成功！"));
			return 0;
		}
		Logger::Message(QString("输入文件错误或者计算失败！"));
	}else{
		Logger::Message(QString("数据库文件或者参数文件解析不正确！"));
	}
	
	return a.exec();
}
