
#include <QtCore/QCoreApplication>
#include "FileReader.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	//! 当前工作路径

	//!! 日志系统

	//! 读取文件
	FileReader fReader = new FileReader(nullptr);

	//! 计算输出

	return a.exec();
}
