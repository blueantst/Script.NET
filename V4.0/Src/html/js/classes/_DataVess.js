/*******************************************\
  游戏人数据操作类 V5.1.5 (2005-xx-xx)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.DataVess = function (fields)
{
	this.count = 0;
	this.recordRow = 0;
	this.structure = fields;
	this.dataBody = [];
	this.status = "unload";
	this.getFieldSortBy = new Array();
	for(var i=0;i<fields.length;i++)
	this.getFieldSortBy[fields[i]] = i;
	this.load = function (tableData)
	{
		this.dataBody = tableData;
		this.count = this.dataBody.length;
		this.status = "loaded";
	}
	this.loadFrom = function (url)
	{
		var newScript = document.createElement("script");
		newScript.type = "text/javascript";
		newScript.src = url;
		document.body.appendChild(newScript);
	}
	this.go = function (n)
	{
		this.recordRow = n;
	}
	this.read = function (fieldName)
	{
		var dat = this.dataBody;
		var record = dat[this.recordRow];
		if (typeof (fieldName)=="undefined") return record;
		var fieldSort = this.getFieldSortBy[fieldName];
		return record[fieldSort];
	}
	this.reads = function (fieldName)
	{
		var re = [];
		for (var i=0; i<this.count; i++)
		{
			this.go(i);
			re[re.length] = this.read(fieldName);
		}
		return re;
	}
	this.append = function (record)
	{
		this.dataBody = this.dataBody.concat([record]);
		this.count = this.dataBody.length;
	}
	this.insert = function (n,record)
	{
		var part1 = this.dataBody.slice(0,n);
		var part2 = this.dataBody.slice(n,this.count);
		this.dataBody = part1.concat([record]).concat(part2);
		this.count = this.dataBody.length;
	}
	this.del = function (n)
	{
		if(typeof(n)=="undefined")
		n = this.recordRow;
		var m = new Array();
		(typeof(n)=="object")?(m=n):(m[0]=n);
		m.sort(function(a,b){return b-a});
		for(var i=0;i<m.length;i++)
		{
			var part1 = this.dataBody.slice(0,m[i]);
			var part2 = this.dataBody.slice(m[i]+1,this.count);
			this.dataBody = part1.concat(part2);
		}
		this.count = this.dataBody.length;
	}
	this.edit = function (fieldName,fieldValue)
	{
		var n = this.recordRow;
		var fieldSort = this.getFieldSortBy[fieldName];
		this.dataBody[n][fieldSort] = fieldValue;
	}
	this.search = function (fieldName,condition,onlyone)
	{
		var co,re=[];
		for(var i=0;i<this.count;i++)
		{
			this.go(i);
			co = "this.read(\"" +fieldName+ "\")" + condition;
			if (eval(co))
			{
				re=re.concat([i]);
				if (onlyone) break;
			}
		}
		return re;
	}
	this.copy = function ()
	{
		var re = new DronFw.Class.DataVess(this.structure);
		re.dataBody = this.dataBody;
		re.count = this.count;
		re.status = this.status;
		return re;
	}
}