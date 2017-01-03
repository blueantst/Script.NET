/*******************************************\
  游戏人日期选择类(2006-06-27)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.Calendar = function (obj)
{
	var o = this;
	var nowIs = new Date();
	var yy = nowYear = nowIs.getFullYear();
	var mm = nowMonth = nowIs.getMonth() + 1;
	var dd = nowDate = nowIs.getDate();
	o.Element = window.document.getElementById(obj);
	o.Element.onfocus = function ()
	{
		o.DivObject.style.left = this.offsetLeft + "px";
		o.DivObject.style.top = this.offsetTop + this.offsetHeight + "px";
		o.DivObject.style.display = "block";
	}
	o.getDaysCount = function (yea,mon){return (new Date(yea,mon,0)).getDate();}
	o.getFirstDay = function (yea,mon){return (new Date(yea,mon-1,1)).getDay();}
	o.innerHTMLToTd = function (n,str)
	{
		var tds = o.DivObject.getElementsByTagName("td");
		tds[n+8].innerHTML = str;
		tds[n+8].onclick = (str=="&nbsp;")?null:function ()
		{
			dd = this.innerHTML.replace(/[^\d]/g,"");
			o.Element.value = yy + "-" + mm + "-" + dd;
			o.DivObject.style.display = "none";
		};
	}
	o.showDayLists = function (yea,mon)
	{
		var ms = o.getDaysCount(yea,mon);	
		var fd = o.getFirstDay(yea,mon);
		for(var i=0;i<42;i++)
		o.innerHTMLToTd(i,"&nbsp;");
		for(var i=1;i<=ms;i++)
		o.innerHTMLToTd(i+fd-1,((nowYear==yea&&nowMonth==mon&&nowDate==i)?("<font color=\"red\">"+i+"</b>"):i));
	}
	o.init = function ()
	{
		var s = "<table cellspacing=\"1\" cellpadding=\"0\" border=\"0\" width=\"148\" height=\"161\" bgcolor=\"#EFEAE1\" style=\"cursor:default;width:150px;height:163px;border-left:2px solid #e2e0da;border-top:2px solid #e2e0da;border-right:2px solid #878583;border-bottom:2px solid #878583;\">\
			<tr height=\"20\" bgcolor=\"#A19E98\">\
				<td colspan=\"7\" align=\"center\">\
					<select style=\"width:70px;\" _DronFw_Calendar=\"true\">";
		for(var i=nowYear-50;i<=nowYear+50;i++)
		s = s.concat("<option" + (i==nowYear?" selected=\"selected\"":"") + " value=\"" +i+ "\">" +i+ "年</option>");
		s = s.concat("</select><select style=\"width:50px;\" _DronFw_Calendar=\"true\">");
		for(var i=1;i<=12;i++)
		s = s.concat("<option" +(i==nowMonth?" selected=\"selected\"":"")+ " value=\"" +i+ "\">" +i+ "月</option>");
		s = s.concat("</select><input onclick=\"this.parentNode.parentNode.parentNode.parentNode.parentNode.style.display=\'none\'\" type=\"button\" value=\"×\" style=\"border:1px;width:18px;height:20px;\">\
				</td>\
			</tr>\
			<tr height=\"20\" bgcolor=\"#A19E98\" align=\"center\">");
		for(var i=0;i<7;i++)
		s = s.concat("<td width=\"20\">" +"日一二三四五六".charAt(i)+ "</td>");
		s = s.concat("</tr>");
		for(var i=0;i<6;i++)
		{
			s = s.concat("<tr height=\"20\" bgcolor=\"#D4D0C8\" align=\"center\">\
				<td width=\"20\" bgcolor=\"#C6C2BB\" onmouseover=\"this.style.backgroundColor=\'#ffffff\'\" onmouseout=\"this.style.backgroundColor=\'\'\"></td>");
			for(var j=0;j<5;j++)
			s = s.concat("<td width=\"20\" onmouseover=\"this.style.backgroundColor=\'#ffffff\'\" onmouseout=\"this.style.backgroundColor=\'\'\"></td>");
			s = s.concat("<td width=\"20\" bgcolor=\"#C6C2BB\" onmouseover=\"this.style.backgroundColor=\'#ffffff\'\" onmouseout=\"this.style.backgroundColor=\'\'\"></td>\
			</tr>");
		}
		s = s.concat("</table>");
		o.DivObject = window.document.createElement("div");
		o.DivObject.style.position = "absolute";
		o.DivObject.style.display = "none";
		window.document.body.appendChild(o.DivObject);
		o.DivObject.innerHTML = s;
		o.showDayLists(yy,mm);
		var sels = o.DivObject.getElementsByTagName("select");
		sels[0].onchange = function (){o.showDayLists(yy=this.value,mm);}
		sels[1].onchange = function (){o.showDayLists(yy,mm=this.value);}
	}
	o.init();
}