#include "ConfigParser.h"

ConfigParser::ConfigParser(std::string const& configFile) {
    std::ifstream file(configFile.c_str());
    if(!file)  {std::cout<<" ***config file not found "<<configFile<<std::endl;}
    std::cout<<" Parser read "<<configFile<<std::endl;

    std::string line;
    std::string name;
    std::string value;
    std::string inSection;
    int posEqual;
    while (std::getline(file,line)) {

        if (! line.length()) continue;

        if (line[0] == '#') continue;
        if (line[0] == ';') continue;

        if (line[0] == '[') {
            inSection=trim(line.substr(1,line.find(']')-1));
            continue;
        }

        posEqual=line.find('=');
        name  = trim(line.substr(0,posEqual));
        value = trim(line.substr(posEqual+1));

        content_[inSection+'/'+name]=Chameleon(value);
    }


    Debug=0;
    //  Debug=this->Value("general","Debug");
    //  cout<<"Config: Debug "<<Debug<<endl;

}
//
Chameleon const& ConfigParser::Value(std::string const& section, std::string const& entry) const{
    std::map<std::string,Chameleon>::const_iterator ci = content_.find(section + '/' + entry);
    if (ci == content_.end()) {
          if(Debug>1)std::cout<<"******no parameter: "<<entry<<" in config section: "<<section<<std::endl;
        return empty;
    }
    return ci->second;
}

int  ConfigParser::Find(std::string const& section, std::string const& entry) const{
    std::map<std::string,Chameleon>::const_iterator ci = content_.find(section + '/' + entry);
    if (ci == content_.end()) {  std::cout<<"******no parameter: "<<entry<<" in config section: "<<section<<std::endl;
        return 0;
    }
    return 1;
}

Chameleon const& ConfigParser::Value(std::string const& section, std::string const& entry, std::string const& value) {
    try {
        return Value(section, entry);
    } catch(const char *) {
        return content_.insert(std::make_pair(section+'/'+entry, Chameleon(value))).first->second;
    }
}

