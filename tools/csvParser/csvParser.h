#ifndef csv_h
#define csv_h

#include <string>
#include <vector>

using namespace std;
namespace CSVParser{

	

	class Row{

	public:
		Row(){};
		~Row(){};

		void pushBack(const string & value){ _values.push_back(value); }
		void setHeader(const vector<string>* header ){ _header = header; }

	public:
		
		int size(){ return _values.size(); }

		string operator [] (int key){
			if (key<0 || key>= size()) throw "key out of range";
			else return _values[key];
		}

		string operator [] (string key){
			vector<string>::const_iterator it;
			int pos = 0;
			for (it = (*_header).begin(); it != (*_header).end(); ++it){
				if (key == *it) return _values[pos];
				pos++;
			}
			throw "can't find ";
		}


	private :
		vector<string> _values;
		const vector<string> * _header;
	};



	class CSV{
	public:
		CSV(){}
		~CSV(){}

		void parse(const string& text);
		void parseFromFile(const string& filename);

		const string& getErrorInfo(){ return _errorMessage; }
		int getRowSize(){ return _values.size(); }
		int getColumnSize(){ return _head.size(); }

		void setHeader();

		Row& operator [](int key){
			if (key < _values.size()) return _values[key];
			throw "doesn't exist this row";
		}


	private:
		vector<Row> _values;
		vector<string> _head;
		string _errorMessage;
	};

}



#endif