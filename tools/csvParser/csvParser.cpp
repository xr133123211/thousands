#include "csvParser.h"
using namespace CSVParser;

void 
CSV::setHeader()
{
	_head.clear();
	for (int i = 0; i < _values[0].size(); i++) {
		_head.push_back(_values[0][i]);
	}
	for (int i = 0; i < _values.size(); i++) {
		_values[i].setHeader(&_head);
	}
}




void  
CSV::parse(const string& text)
{
	enum stateType{
		Normal,
		QuotesField,
		FieldSperator,
		QuotoInQuotesField,
		RowSeperator,
		Error
	};

	_values.clear();
	_errorMessage.clear();
	Row Fields = Row();
	string strField = "";
	stateType state = Normal;
	int size;

	if (text.size() == 0) return;


	for (int i = 0, size = text.size(); i < size; ++i){
		const char & ch = text[i];	
		switch (state)
		{
		case Normal:
			{
					   if (ch == ','){ state = FieldSperator; }
					   else if (ch == '"'){ state = QuotesField; }
					   else if (ch == '\r'){ state = RowSeperator; }
					   else strField.push_back(ch);
			}
			break;

		case QuotesField:
			{
							if (ch == '"'){ state = QuotoInQuotesField; }
							else{ strField.push_back(ch); }
			}
			break;

		case FieldSperator:
			{
							  Fields.pushBack(strField);
							  strField.clear();
							  if (ch == '\r') state = RowSeperator;
							  else if (ch == '"') state = QuotesField;
							  else {
								  strField.push_back(ch);
								  state = Normal;
							  }
			}
			break;

		case QuotoInQuotesField:
			{
								   if (ch == '"') { strField.push_back(ch); state = QuotesField; }
								   else if (ch == ',') { state = FieldSperator; }
								   else if (ch == '\r') { state = RowSeperator; }
								   else {
									   state = Error;
									   _errorMessage = "\" missed or left";
								   }
			}
			break;

		case RowSeperator:
			{
							 if (ch == '\n') {
								 Fields.pushBack(strField);								 
								 strField.clear();								 
								 _values.push_back(Fields);
								 Fields = Row();
								 state = Normal;
							 }
							 else {
								 state = Error;
								 _errorMessage = "not \\n after \\r";
							 }
			}
			break;

		case Error:
			{
					  i = size;

			}
			break;
		default:break;
		}
	}		
	//end for 

	if (state == FieldSperator) {
			Fields.pushBack(strField);
			Fields.pushBack("");
			_values.push_back(Fields);
	}
	else if (state == QuotoInQuotesField){
			Fields.pushBack(strField);
			_values.push_back(Fields);
	}
}

void
CSV::parseFromFile(const string& filename){
	FILE* file = fopen(filename.c_str(),"rb");
	if (!file) return;

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	
	fseek(file, 0, SEEK_SET);

	char* bufferChar= new char[size+1];
	fread(bufferChar, 1, size, file);
	fclose(file);
	string text;
	text.assign(bufferChar, size);
	delete[](bufferChar);
	
	parse(text);


}

