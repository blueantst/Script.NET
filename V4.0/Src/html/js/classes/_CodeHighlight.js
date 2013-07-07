/**//**//**//**//**//**//**//*  
**    ==================================================================================================  
**    类名：CLASS_HIGHLIGHT  
**    功能：语法高亮  
**    示例：  
    ---------------------------------------------------------------------------------------------------  
  
            var xx        = new CLASS_HIGHLIGHT(code,syntax);            
            document.getElementById("display").innerHTML = xx.highlight(); 
  
    ---------------------------------------------------------------------------------------------------  
**    作者：ttyp  
**    邮件：ttyp@21cn.com  
**    日期：2005-7-31
**    其它说明：本类不是 Dron 原创，这里改成这种形式只是方便 DronFw 使用，感谢原作者 ttyp
**    ==================================================================================================  
**/  
DronFw.Class.CodeHighlight = function (code,syntax) 
{
    //哈希表类
    function Hashtable()
    {
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
    //字符串转换为哈希表
    this.str2hashtable = function(key,cs){
        
        var _key    = key.split(/,/g);
        var _hash    = new Hashtable();
        var _cs        = true;
        if(typeof(cs)=="undefined"){
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

    switch(syntax.toLowerCase())
    {
        case "sql":
            //是否大小写敏感
            this._caseSensitive    = false;
            //得到关键字哈希表
            this._keywords        = this.str2hashtable("COMMIT,DELETE,INSERT,LOCK,ROLLBACK,SELECT,TRANSACTION,READ,ONLY,WRITE,USE,ROLLBACK,SEGMENT,ROLE,EXCEPT,NONE,UPDATE,DUAL,WORK,COMMENT,FORCE,FROM,WHERE,INTO,VALUES,ROW,SHARE,MODE,EXCLUSIVE,UPDATE,ROW,NOWAIT,TO,SAVEPOINT,UNION,UNION,ALL,INTERSECT,MINUS,START,WITH,CONNECT,BY,GROUP,HAVING,ORDER,UPDATE,NOWAIT,IDENTIFIED,SET,DROP,PACKAGE,CREATE,REPLACE,PROCEDURE,FUNCTION,TABLE,RETURN,AS,BEGIN,DECLARE,END,IF,THEN,ELSIF,ELSE,WHILE,CURSOR,EXCEPTION,WHEN,OTHERS,NO_DATA_FOUND,TOO_MANY_ROWS,CURSOR_ALREADY_OPENED,FOR,LOOP,IN,OUT,TYPE,OF,INDEX,BINARY_INTEGER,RAISE,ROWTYPE,VARCHAR2,NUMBER,LONG,DATE,RAW,LONG RAW,CHAR,INTEGER,MLSLABEL,CURRENT,OF,DEFAULT,CURRVAL,NEXTVAL,LEVEL,ROWID,ROWNUM,DISTINCT,ALL,LIKE,IS,NOT,NULL,BETWEEN,ANY,AND,OR,EXISTS,ASC,DESC,ABS,CEIL,COS,COSH,EXP,FLOOR,LN,LOG,MOD,POWER,ROUND,SIGN,SIN,SINH,SQRT,TAN,TANH,TRUNC,CHR,CONCAT,INITCAP,LOWER,LPAD,LTRIM,NLS_INITCAP,NLS_LOWER,NLS_UPPER,REPLACE,RPAD,RTRIM,SOUNDEX,SUBSTR,SUBSTRB,TRANSLATE,UPPER,ASCII,INSTR,INSTRB,LENGTH,LENGTHB,NLSSORT,ADD_MONTHS,LAST_DAY,MONTHS_BETWEEN,NEW_TIME,NEXT_DAY,ROUND,SYSDATE,TRUNC,CHARTOROWID,CONVERT,HEXTORAW,RAWTOHEX,ROWIDTOCHAR,TO_CHAR,TO_DATE,TO_LABEL,TO_MULTI_BYTE,TO_NUMBER,TO_SINGLE_BYTE,DUMP,GREATEST,GREATEST_LB,LEAST,LEAST_UB,NVL,UID,USER,USERENV,VSIZE,AVG,COUNT,GLB,LUB,MAX,MIN,STDDEV,SUM,VARIANCE");
            //得到内建对象哈希表
            this._commonObjects = this.str2hashtable("");
            //标记
            this._tags            = this.str2hashtable("",false);
            //得到分割字符
            this._wordDelimiters= "　 ,.?!;:\\/<>(){}[]\"'\r\n\t=+-|*%@#$^&";
            //引用字符
            this._quotation        = this.str2hashtable("'");
            //行注释字符
            this._lineComment    = "--";
            //转义字符
            this._escape        = "";
            //多行引用开始
            this._commentOn        = "/**//**//**//**//**//**//**//*";
            //多行引用结束
            this._commentOff    = "*/";
            //忽略词
            this._ignore        = "";    
            //是否处理标记
            this._dealTag        = false;
            break;
        case "c#":
            //是否大小写敏感
            this._caseSensitive    = true;
            //得到关键字哈希表
            this._keywords        = this.str2hashtable("abstract,as,base,bool,break,byte,case,catch,char,checked,class,const,continue,decimal,default,delegate,do,double,else,enum,event,explicit,extern,false,finally,fixed,float,for,foreach,get,goto,if,implicit,in,int,interface,internal,is,lock,long,namespace,new,null,object,operator,out,override,params,private,protected,public,readonly,ref,return,sbyte,sealed,short,sizeof,stackalloc,static,set,string,struct,switch,this,throw,true,try,typeof,uint,ulong,unchecked,unsafe,ushort,using,value,virtual,void,volatile,while");
            //得到内建对象哈希表
            this._commonObjects = this.str2hashtable("String,Boolean,DateTime,Int32,Int64,Exception,DataTable,DataReader");
            //标记
            this._tags            = this.str2hashtable("",false);
            //得到分割字符
            this._wordDelimiters= "　 ,.?!;:\\/<>(){}[]\"'\r\n\t=+-|*%@#$^&";
            //引用字符
            this._quotation        = this.str2hashtable("\"");
            //行注释字符
            this._lineComment    = "//";
            //转义字符
            this._escape        = "\\";
            //多行引用开始
            this._commentOn        = "/*";
            //多行引用结束
            this._commentOff    = "*/";
            //忽略词
            this._ignore        = "";                
            //是否处理标记
            this._dealTag        = false;
            break;    
        case "java":
            //是否大小写敏感
            this._caseSensitive    = true;
            //得到关键字哈希表
            this._keywords        = this.str2hashtable("abstract,boolean,break,byte,case,catch,char,class,const,continue,default,do,double,else,extends,final,finally,float,for,goto,if,implements,import,instanceof,int,interface,long,native,new,package,private,protected,public,return,short,static,strictfp,super,switch,synchronized,this,throw,throws,transient,try,void,volatile,while");
            //得到内建对象哈希表
            this._commonObjects = this.str2hashtable("String,Boolean,DateTime,Int32,Int64,Exception,DataTable,DataReader");
            //标记
            this._tags            = this.str2hashtable("",false);
            //得到分割字符
            this._wordDelimiters= "　 ,.?!;:\\/<>(){}[]\"'\r\n\t=+-|*%@#$^&";
            //引用字符
            this._quotation        = this.str2hashtable("\"");
            //行注释字符
            this._lineComment    = "//";
            //转义字符
            this._escape        = "\\";
            //多行引用开始
            this._commentOn        = "/*";
            //多行引用结束
            this._commentOff    = "*/";
            //忽略词
            this._ignore        = "";        
            //是否处理标记
            this._dealTag        = false;            
            break;    
        case "vbs":
        case "vb":
            //是否大小写敏感
            this._caseSensitive    = false;
            //得到关键字哈希表
            this._keywords        = this.str2hashtable("And,ByRef,ByVal,Call,Case,Class,Const,Dim,Do,Each,Else,ElseIf,Empty,End,Eqv,Erase,Error,Exit,Explicit,False,For,Function,Get,If,Imp,In,Is,Let,Loop,Mod,Next,Not,Nothing,Null,On,Option,Or,Private,Property,Public,Randomize,ReDim,Resume,Select,Set,Step,Sub,Then,To,True,Until,Wend,While,Xor,Anchor,Array,Asc,Atn,CBool,CByte,CCur,CDate,CDbl,Chr,CInt,CLng,Cos,CreateObject,CSng,CStr,Date,DateAdd,DateDiff,DatePart,DateSerial,DateValue,Day,Dictionary,Document,Element,Err,Exp,FileSystemObject,Filter,Fix,Int,Form,FormatCurrency,FormatDateTime,FormatNumber,FormatPercent,GetObject,Hex,Hour,InputBox,InStr,InstrRev,IsArray,IsDate,IsEmpty,IsNull,IsNumeric,IsObject,Join,LBound,LCase,Left,Len,Link,LoadPicture,Location,Log,LTrim,RTrim,Trim,Mid,Minute,Month,MonthName,MsgBox,Navigator,Now,Oct,Replace,Right,Rnd,Round,ScriptEngine,ScriptEngineBuildVersion,ScriptEngineMajorVersion,ScriptEngineMinorVersion,Second,Sgn,Sin,Space,Split,Sqr,StrComp,String,StrReverse,Tan,Time,TextStream,TimeSerial,TimeValue,TypeName,UBound,UCase,VarType,Weekday,WeekDayName,Year");
            //得到内建对象哈希表
            this._commonObjects = this.str2hashtable("String,Number,Boolean,Date,Integert,Long,Double,Single");
            //标记
            this._tags            = this.str2hashtable("",false);
            //得到分割字符
            this._wordDelimiters= "　 ,.?!;:\\/<>(){}[]\"'\r\n\t=+-|*%@#$^&";
            //引用字符
            this._quotation        = this.str2hashtable("\"");
            //行注释字符
            this._lineComment    = "'";
            //转义字符
            this._escape        = "";
            //多行引用开始
            this._commentOn        = "";
            //多行引用结束
            this._commentOff    = "";
            //忽略词
            this._ignore        = "<!--";    
            //是否处理标记
            this._dealTag        = false;
            break;
        case "js":
            //是否大小写敏感
            this._caseSensitive    = true;
            //得到关键字哈希表
            this._keywords        = this.str2hashtable("function,void,this,boolean,while,if,return,new,true,false,try,catch,throw,null,else,int,long,do,var");
            //得到内建对象哈希表
            this._commonObjects = this.str2hashtable("String,Number,Boolean,RegExp,Error,Math,Date");
            //标记
            this._tags            = this.str2hashtable("",false);
            //得到分割字符
            this._wordDelimiters= "　 ,.?!;:\\/<>(){}[]\"'\r\n\t=+-|*%@#$^&";
            //引用字符
            this._quotation        = this.str2hashtable("\",'");
            //行注释字符
            this._lineComment    = "//";
            //转义字符
            this._escape        = "\\";
            //多行引用开始
            this._commentOn        = "/*";
            //多行引用结束
            this._commentOff    = "*/";
            //忽略词
            this._ignore        = "<!--";
            break;
        case "html":
            //是否大小写敏感
            this._caseSensitive    = true;
            //得到关键字哈希表
            this._keywords        = this.str2hashtable("function,void,this,boolean,while,if,return,new,true,false,try,catch,throw,null,else,int,long,do,var");
            //得到内建对象哈希表
            this._commonObjects = this.str2hashtable("String,Number,Boolean,RegExp,Error,Math,Date");
            //标记
            this._tags            = this.str2hashtable("html,head,body,title,style,script,language,input,select,div,span,button,img,iframe,frame,frameset,table,tr,td,caption,form,font,meta,textarea",false);
            //得到分割字符
            this._wordDelimiters= "　 ,.?!;:\\/<>(){}[]\"'\r\n\t=+-|*%@#$^&";
            //引用字符
            this._quotation        = this.str2hashtable("\",'");
            //行注释字符
            this._lineComment    = "//";
            //转义字符
            this._escape        = "\\";
            //多行引用开始
            this._commentOn        = "/*";
            //多行引用结束
            this._commentOff    = "*/";
            //忽略词
            this._ignore        = "<!--";
            //是否处理标记
            this._dealTag        = true;
            break;
        case "xml":
        default:
            //是否大小写敏感
            this._caseSensitive    = true;
            //得到关键字哈希表
            this._keywords        = this.str2hashtable("!DOCTYPE,?xml,script,version,encoding");
            //得到内建对象哈希表
            this._commonObjects = this.str2hashtable("");
            //标记
            this._tags            = this.str2hashtable("",false);
            //得到分割字符
            this._wordDelimiters= "　 ,.;:\\/<>(){}[]\"'\r\n\t=+-|*%@#$^&";
            //引用字符
            this._quotation        = this.str2hashtable("\",'");
            //行注释字符
            this._lineComment    = "";
            //转义字符
            this._escape        = "\\";
            //多行引用开始
            this._commentOn        = "<!--";
            //多行引用结束
            this._commentOff    = "-->";
            //忽略词
            this._ignore        = "<!--";
            //是否处理标记
            this._dealTag        = true;
            break;
    }
    
    this.highlight    = function() {
        var codeArr = new Array();
        var word_index = 0;
        var htmlTxt = new Array();
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
        var slash_star_comment_opened    = false;    //多行注释标记
        var slash_slash_comment_opened    = false;    //单行注释标记
        var line_num                    = 1;        //行号
        var quote_char                    = "";        //引用标记类型
        var tag_opened                    = false;    //标记开始
        htmlTxt[htmlTxt.length] = ("<div style='font-family: Courier New;font-size:12px;overflow:auto;border-width:1px;border-style:solid;border-color:#8a8a8a;background-color:#eeeeee;margin:1px;padding:6px;'>");
        //按分割字，分块显示
        for (var i=0; i <=word_index; i++){
            //处理空行（由于转义带来）
            if(typeof(codeArr[i])=="undefined"||codeArr[i].length==0){
                continue;
            }
            //处理空格
            if (codeArr[i] == " "){                                                                        
                htmlTxt[htmlTxt.length] = ("&nbsp;");
            //处理关键字
            } else if (!slash_slash_comment_opened&&!slash_star_comment_opened && !quote_opened && this.isKeyword(codeArr[i])){        
                htmlTxt[htmlTxt.length] = ("<span style='color:#0000FF;'>" + codeArr[i] + "</span>");
            //处理普通对象
            } else if (!slash_slash_comment_opened&&!slash_star_comment_opened && !quote_opened && this.isCommonObject(codeArr[i])){    
                htmlTxt[htmlTxt.length] = ("<span style='color:#808000;'>" + codeArr[i] + "</span>");
            //处理标记
            } else if (!slash_slash_comment_opened&&!slash_star_comment_opened && !quote_opened && tag_opened && this.isTag(codeArr[i])){    
                htmlTxt[htmlTxt.length] = ("<span style='color:#0000FF;'>" + codeArr[i] + "</span>");
            //处理换行
            } else if (codeArr[i] == "\n"){                                                                
                if (slash_slash_comment_opened){
                    htmlTxt[htmlTxt.length] = ("</span>");    
                    slash_slash_comment_opened = false;
                }
                htmlTxt[htmlTxt.length] = ("<br/>");
                line_num++;
            //处理双引号（引号前不能为转义字符）    
            } else if (this._quotation.contains(codeArr[i])&&!slash_star_comment_opened&&!slash_slash_comment_opened){                                        
                if (quote_opened){
                    //是相应的引号
                    if(quote_char==codeArr[i]){
                        if(tag_opened){
                            htmlTxt[htmlTxt.length] = (codeArr[i]+"</span><span style='color:#808000;'>");                            
                        } else {
                            htmlTxt[htmlTxt.length] = (codeArr[i]+"</span>");
                        }
                        quote_opened    = false;
                        quote_char        = "";
                    } else {
                        htmlTxt[htmlTxt.length] = codeArr[i].replace(/\</g,"&lt;");
                    }
                } else {
                    if(tag_opened){
                        htmlTxt[htmlTxt.length] =  ("</span><span style='color:#FF00FF;'>"+codeArr[i]);
                    } else {
                        htmlTxt[htmlTxt.length] =  ("<span style='color:#FF00FF;'>"+codeArr[i]);
                    }
                    quote_opened    = true;
                    quote_char        = codeArr[i];
                }                    
            //处理转义字符
            } else if(codeArr[i] == this._escape){    
                htmlTxt[htmlTxt.length] = (codeArr[i]); 
                if(i<word_index-1){
                    if(codeArr[i+1].charCodeAt(0)>=32&&codeArr[i+1].charCodeAt(0)<=127){
                        htmlTxt[htmlTxt.length] = codeArr[i+1].substr(0,1).replace("&","&amp;").replace(/\</g,"&lt;"); 
                        codeArr[i+1] = codeArr[i+1].substr(1);                         
                    }
                }            
            //处理Tab
            } else if (codeArr[i] == "\t") {                            
                htmlTxt[htmlTxt.length] = ("&nbsp;&nbsp;&nbsp;&nbsp;");
            //处理多行注释的开始
            } else if (this.isStartWith(this._commentOn,codeArr,i)&&!slash_slash_comment_opened && !slash_star_comment_opened&&!quote_opened){                                                
                slash_star_comment_opened = true;
                htmlTxt[htmlTxt.length] =  ("<span style='color:#008000;'>" + this._commentOn.replace(/\</g,"&lt"));
                i = i + this._commentOn.length-1;        
            //处理单行注释
            } else if (this.isStartWith(this._lineComment,codeArr,i)&&!slash_slash_comment_opened && !slash_star_comment_opened&&!quote_opened){                                                
                slash_slash_comment_opened = true;
                htmlTxt[htmlTxt.length] =  ("<span style='color:#008000;'>" + this._lineComment);
                i = i + this._lineComment.length-1;    
            //处理忽略词
            } else if (this.isStartWith(this._ignore,codeArr,i)&&!slash_slash_comment_opened && !slash_star_comment_opened&&!quote_opened){                                                
                slash_slash_comment_opened = true;
                htmlTxt[htmlTxt.length] =  ("<span style='color:#008000;'>" + this._ignore.replace(/\</g,"&lt"));
                i = i + this._ignore.length-1;                    
            //处理多行注释结束    
            } else if (this.isStartWith(this._commentOff,codeArr,i)&&!quote_opened&&!slash_slash_comment_opened){                                        
                if (slash_star_comment_opened) {
                    slash_star_comment_opened = false;
                    htmlTxt[htmlTxt.length] =  (this._commentOff +"</span>");
                    i = i + this._commentOff.length-1;        
                }
            //处理左标记
            } else if (this._dealTag&&!slash_slash_comment_opened && !slash_star_comment_opened&&!quote_opened&&codeArr[i] == "<") {                
                htmlTxt[htmlTxt.length] = "&lt;<span style='color:#808000;'>";
                tag_opened    = true;
            //处理右标记
            } else if (this._dealTag&&tag_opened&&codeArr[i] == ">") {                                
                htmlTxt[htmlTxt.length] = "</span>&gt;";
                tag_opened    = false;
            //处理HTML转义符号
            } else if (codeArr[i] == "&") {                                
                htmlTxt[htmlTxt.length] = "&amp;";
            } else {
                htmlTxt[htmlTxt.length] = codeArr[i].replace(/\</g,"&lt;");
            }
            
        }
        htmlTxt[htmlTxt.length] = ("</div>");
        return htmlTxt.join("");
    }
    this.isStartWith = function(str,code,index){
        if(typeof(str)!="undefined"&&str.length>0){
            for(var i=0;i<str.length;i++){
                if(this._caseSensitive){
                    if(str.charAt(i)!=code[index+i]||(index+i>=code.length)){
                        return false;
                    }
                } else {
                    if(str.charAt(i).toLowerCase()!=code[index+i].toLowerCase()||(index+i>=code.length)){
                        return false;
                    }
                }
            }
            return true;
        } else {
            return false;
        }
    }
    
    this.isKeyword = function(val) {        
        return this._keywords.contains(this._caseSensitive?val:val.toLowerCase());
    }
    this.isCommonObject = function(val) {
        return this._commonObjects.contains(this._caseSensitive?val:val.toLowerCase());
    }
    this.isTag = function(val) {
        return this._tags.contains(val.toLowerCase());
    }
}