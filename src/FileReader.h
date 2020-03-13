#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QString>

class FileReader : public QObject
{
	Q_OBJECT

public:
	FileReader(QObject *parent);
	~FileReader();

	//! 1、读取数据库样本库文件
	bool ReadDatabase(QString fileName);

	//! 2、读取控制参数文件
	bool ReadDatabaseParam(QString fileName);

	//! 3、读取输入文件并计算输出
	bool ReadInput(QString fileName);
private:
	
};

#endif // FILEREADER_H
