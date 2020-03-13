
#include <QtCore/QCoreApplication>
#include <QDir>
#include <QFile>


#include "FileReader.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

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

	//!! 日志系统，存储到log目录下

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
			return 0;
		}
	}
	
	return a.exec();
}
