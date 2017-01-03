/**//**//**//** 
**    ==================================================================================================  
**    类名：CLASS_FORMATER
**    功能：JS格式化  
**    示例：  
    ---------------------------------------------------------------------------------------------------  
  
            var xx        = new CLASS_FORMATER(code);            

            document.getElementById("display").innerHTML = xx.format(); 
  
    ---------------------------------------------------------------------------------------------------  
**    作者：ttyp  
**    邮件：ttyp@21cn.com  
**    日期：2006-5-21  
**    版本：0.1
**    其它说明：本例并非 Dron 原创，这里改成这个形式是为了给 DronFw 使用，感谢原作者 ttyp。
**    ==================================================================================================  
**/  
DronFw.Class.CodeFormat = function(code){
    //哈希表类
    function Hashtable(){
        this._hash        = new Object();
        this.add        = function(key,value){
                            if(typeof(key)!="undefined"){
                                if(this.contains(key)==false){
                                    this._hash[key]=typeof(value)=="undefined"?null:value;
                                    return true;
                                } else {
                                    return false;
                                }
                            } else {
                                return false;
                            }
                        }
        this.remove        = function(key){delete this._hash[key];}
        this.count        = function(){var i=0;for(var k in this._hash){i++;} return i;}
        this.items        = function(key){return this._hash[key];}
        this.contains    = function(key){return typeof(this._hash[key])!="undefined";}
        this.clear        = function(){for(var k in this._hash){delete this._hash[k];}}

    }

    this._caseSensitive = true;

    //字符串转换为哈希表
    this.str2hashtable = function(key,cs){
        
        var _key    = key.split(/,/g);
        var _hash    = new Hashtable(); 
        var _cs        = true;

    
        if(typeof(cs)=="undefined"||cs==null){
            _cs = this._caseSensitive;
        } else {
            _cs = cs;
        }

        for(var i in _key){
            if(_cs){
                _hash.add(_key[i]);
            } else {
                _hash.add((_key[i]+"").toLowerCase());
            }

        }
        return _hash;
    }

    //获得需要转换的代码
    this._codetxt        = code;

    if(typeof(syntax)=="undefined"){
        syntax = "";
    }

    this._deleteComment = false;
    //是否大小写敏感
    this._caseSensitive = true;
    //可以后面加块语句的关键字
    this._blockElement  = this.str2hashtable("switch,if,while,try,finally");
    //是函数申明
    this._function      = this.str2hashtable("function");
    //本行括号内分号不做换行
    this._isFor            = "for";

    this._choiceElement = this.str2hashtable("else,catch");

    this._beginBlock    = "{";
    this._endBlock      = "}";
    
    this._singleEyeElement = this.str2hashtable("var,new,return,else,delete,in,case");
    //得到分割字符
    this._wordDelimiters= "　 ,.?!;:\\/<>(){}[]\"'\r\n\t=+-|*%@#$^&";
    //引用字符
    this._quotation     = this.str2hashtable("\",'");
    //行注释字符
    this._lineComment   = "//";
    //转义字符
    this._escape        = "\\";
    //多行引用开始
    this._commentOn        = "/*";
    //多行引用结束
    this._commentOff    = "*/";
    //行结束词
    this._rowEnd        = ";";

    this._in            = "in";


    this.isCompress        = false;
    this.style            = 0;

    this._tabNum        = 0;


    this.format = function() {
        var codeArr        = new Array();
        var word_index    = 0;
        var htmlTxt        = new Array();

        if(this.isCompress){
            this._deleteComment = true;
        }


        //得到分割字符数组(分词)
        for (var i = 0; i < this._codetxt.length; i++) {       
            if (this._wordDelimiters.indexOf(this._codetxt.charAt(i)) == -1) {        //找不到关键字
                if (codeArr[word_index] == null || typeof(codeArr[word_index]) == 'undefined') {
                    codeArr[word_index] = "";
                }
                codeArr[word_index] += this._codetxt.charAt(i);
            } else {
                if (typeof(codeArr[word_index]) != 'undefined' && codeArr[word_index].length > 0)
                    word_index++;
                codeArr[word_index++] = this._codetxt.charAt(i);                
            } 
        }


        var quote_opened                = false;    //引用标记
        var slash_star_comment_opened   = false;    //多行注释标记
        var slash_slash_comment_opened  = false;    //单行注释标记
        var line_num                    = 1;        //行号
        var quote_char                  = "";       //引用标记类型

        var function_opened             = false;

        var bracket_open                = false;
        var for_open                    = false;

        //按分割字，分块显示
        for (var i=0; i <=word_index; i++){            
            //处理空行（由于转义带来）
            if(typeof(codeArr[i])=="undefined"||codeArr[i].length==0){
                continue;
            } else if(codeArr[i]==" "||codeArr[i]=="\t"){
                if(slash_slash_comment_opened||slash_star_comment_opened){
                    if(!this._deleteComment){
                        htmlTxt[htmlTxt.length] = codeArr[i];
                    }
                }
                if(quote_opened){
                        htmlTxt[htmlTxt.length] = codeArr[i];                    
                }
            } else if(codeArr[i]=="\n"){
            //处理换行
            } else if (codeArr[i] == "\r"){                                                                    
                slash_slash_comment_opened = false;    
                quote_opened    = false;
                line_num++;
                if(!this.isCompress){
                    htmlTxt[htmlTxt.length] = "\r\n"+ this.getIdent();    
                }
            //处理function里的参数标记
            } else if (!slash_slash_comment_opened&&!slash_star_comment_opened && !quote_opened &&this.isFunction(codeArr[i])){
                htmlTxt[htmlTxt.length] = codeArr[i]  + " ";
                function_opened = true;
            } else if (!slash_slash_comment_opened&&!slash_star_comment_opened && !quote_opened &&codeArr[i]==this._isFor){
                htmlTxt[htmlTxt.length] = codeArr[i];
                for_open = true;
            } else if (!slash_slash_comment_opened&&!slash_star_comment_opened && !quote_opened &&codeArr[i]=="("){
                bracket_open    = true;
                htmlTxt[htmlTxt.length] = codeArr[i];
            } else if (!slash_slash_comment_opened&&!slash_star_comment_opened && !quote_opened &&codeArr[i]==")"){
                bracket_open    = false;
                htmlTxt[htmlTxt.length] = codeArr[i];
            } else if (!slash_slash_comment_opened&&!slash_star_comment_opened && !quote_opened &&codeArr[i]==this._rowEnd){
                if(!this.isCompress){
                    if(!for_open){
                        if(i<word_index&&(codeArr[i+1]!="\r"&&codeArr[i+1]!="\n")){                            
                            htmlTxt[htmlTxt.length] = codeArr[i] + "\n" + this.getIdent();
                        }else{
                            htmlTxt[htmlTxt.length] = codeArr[i] + this.getIdent();
                        }
                    }else{
                        htmlTxt[htmlTxt.length] = codeArr[i];                    
                    }
                }else{
                    htmlTxt[htmlTxt.length] = codeArr[i];
                }
            } else if(!slash_slash_comment_opened&&!slash_star_comment_opened && !quote_opened &&codeArr[i]==this._beginBlock){
                for_open    = false;
                if(!this.isCompress){
                    switch(this.style){
                        case 0:
                            this._tabNum++;
                            htmlTxt[htmlTxt.length] = codeArr[i] + "\n" + this.getIdent();
                            break;
                        case 1:
                            htmlTxt[htmlTxt.length] = "\n" + this.getIdent();
                            this._tabNum++;
                            htmlTxt[htmlTxt.length] = codeArr[i] + "\n"+ this.getIdent();
                            break;
                        default:
                            this._tabNum++;
                            htmlTxt[htmlTxt.length] = codeArr[i];
                            break;
                            
                    }
                }else{
                    htmlTxt[htmlTxt.length] = codeArr[i];
                }

            } else if(!slash_slash_comment_opened&&!slash_star_comment_opened && !quote_opened &&codeArr[i]==this._endBlock){
                if(!this.isCompress){
                    this._tabNum--;
                    if(i<word_index&&codeArr[i+1]!=this._rowEnd){
                        htmlTxt[htmlTxt.length] = "\n" + this.getIdent() + codeArr[i];
                    }else{
                        htmlTxt[htmlTxt.length] = "\n" + this.getIdent() + codeArr[i];
                    }
                }else{
                    if(i<word_index&&codeArr[i+1]!=this._rowEnd){
                        htmlTxt[htmlTxt.length] = codeArr[i] + this._rowEnd;
                    }else{
                        htmlTxt[htmlTxt.length] = codeArr[i];
                    }
                }
            //处理关键字
            } else if (!slash_slash_comment_opened&&!slash_star_comment_opened && !quote_opened && this.isBlockElement(codeArr[i])){     
                htmlTxt[htmlTxt.length] = codeArr[i];
            //处理内置对象(后面加一个空格)
            } else if (!slash_slash_comment_opened&&!slash_star_comment_opened && !quote_opened && this.isSingleEyeElement(codeArr[i])){    
                if(codeArr[i]==this._in){
                    htmlTxt[htmlTxt.length] = " ";
                }
                htmlTxt[htmlTxt.length] = codeArr[i] + " ";
            //处理双引号（引号前不能为转义字符）    
            } else if (!slash_star_comment_opened&&!slash_slash_comment_opened&&this._quotation.contains(codeArr[i])){                                                    
                if (quote_opened){
                    //是相应的引号
                    if(quote_char==codeArr[i]){
                        htmlTxt[htmlTxt.length] = codeArr[i];                    
                        quote_opened    = false;
                        quote_char      = "";
                    } else {
                        htmlTxt[htmlTxt.length] = codeArr[i];            
                    }
                } else {
                    htmlTxt[htmlTxt.length] =  codeArr[i];
                    quote_opened    = true;
                    quote_char        = codeArr[i];
                }                    
            //处理转义字符
            } else if(codeArr[i] == this._escape){    
                htmlTxt[htmlTxt.length] = codeArr[i]; 
                if(i<word_index-1){
                    if(codeArr[i+1].charCodeAt(0)>=32&&codeArr[i+1].charCodeAt(0)<=127){
                        htmlTxt[htmlTxt.length] = codeArr[i+1].substr(0,1);
                        htmlTxt[htmlTxt.length] = codeArr[i+1].substr(1);
                        i=i+1;
                    }
                }            
            //处理多行注释的开始
            } else if (!slash_slash_comment_opened && !slash_star_comment_opened&&!quote_opened&&this.isStartWith(this._commentOn,codeArr,i)){                                             
                slash_star_comment_opened = true;
                if(!this._deleteComment){
                    htmlTxt[htmlTxt.length] = this._commentOn;
                }
                i = i + this.getSkipLength(this._commentOn);    
            //处理单行注释
            } else if (!slash_slash_comment_opened && !slash_star_comment_opened&&!quote_opened&&this.isStartWith(this._lineComment,codeArr,i)){                                                
                slash_slash_comment_opened = true;
                if(!this._deleteComment){
                    htmlTxt[htmlTxt.length] =  this._lineComment;
                }
                i = i + this.getSkipLength(this._lineComment);    
            //处理忽略词
            } else if (!slash_slash_comment_opened && !slash_star_comment_opened&&!quote_opened&&this.isStartWith(this._ignore,codeArr,i)){                                                
                slash_slash_comment_opened = true;
                htmlTxt[htmlTxt.length] = this._ignore;
                i = i + this.getSkipLength(this._ignore);                    
            //处理多行注释结束    
            } else if (!quote_opened&&!slash_slash_comment_opened&&this.isStartWith(this._commentOff,codeArr,i)){                                
                if (slash_star_comment_opened) {
                    slash_star_comment_opened = false;
                    if(!this._deleteComment){
                        htmlTxt[htmlTxt.length] =  this._commentOff;
                    }
                    i = i + this.getSkipLength(this._commentOff);        
                }
            } else {
                //不是在字符串中
                if(!quote_opened){
                    //如果不是在注释重
                    if(!slash_slash_comment_opened && !slash_star_comment_opened){    
                            htmlTxt[htmlTxt.length] = codeArr[i];                        
                    //注释中                            
                    }else{
                        if(!this._deleteComment){
                            htmlTxt[htmlTxt.length] = codeArr[i];
                        }
                    }
                }else{
                            htmlTxt[htmlTxt.length] = codeArr[i];
                }
            }
            
        }

        return htmlTxt.join("");
    }

    this.isStartWith = function(str,code,index){

        if(typeof(str)!="undefined"&&str.length>0){        
            var cc = new Array();            
            for(var i=index;i<index+str.length;i++){
                cc[cc.length] = code[i];
            }
            var c = cc.join("");
            if(this._caseSensitive){
                if(str.length>=code[index].length&&c.indexOf(str)==0){
                    return true;
                }
            }else{
                if(str.length>=code[index].length&&c.toLowerCase().indexOf(str.toLowerCase())==0){
                    return true;
                }
            }
            return false;

        } else {
            return false;
        }
    }

    this.isFunction = function(val){
        return this._function.contains(this._caseSensitive?val:val.toLowerCase());
    }
    
    this.isBlockElement = function(val) {        
        return this._blockElement.contains(this._caseSensitive?val:val.toLowerCase());
    }

    this.isChoiceElement = function(val) {        
        return this._choiceElement.contains(this._caseSensitive?val:val.toLowerCase());
    }

    this.isSingleEyeElement = function(val) {
        return this._singleEyeElement.contains(this._caseSensitive?val:val.toLowerCase());
    }

    this.isNextElement = function(from,word){
        for(var i=from;i<word.length;i++){
            if(word[i]!=" "&&word[i]!="\t"&&word[i]!="\r"&&word[i]!="\n"){                
                return this.isChoiceElement(word[i]);
            }
        }
        return false;
    }

    this.getSkipLength = function(val){    
        var count = 0;
        for(var i=0;i<val.length;i++){
            if(this._wordDelimiters.indexOf(val.charAt(i))>=0){
                count++;
            }
        }
        if(count>0){
            count=count-1;
        }
        return count;
    }

    this.getIdent=function(){
        var n = [];
        for(var i=0;i<this._tabNum;i++){
            n[n.length] = "\t";
        }
        return n.join("");
    }
}