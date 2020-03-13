#include "FileReader.h"

FileReader::FileReader(QObject *parent)
	: QObject(parent)
{

}

FileReader::~FileReader()
{

}

bool FileReader::ReadDatabase(QString fileName)
{

	return false;
}


bool FileReader::ReadDatabaseParam(QString fileName)
{
	return false;
}

bool FileReader::ReadInput(QString fileName)
{
	return false;
}