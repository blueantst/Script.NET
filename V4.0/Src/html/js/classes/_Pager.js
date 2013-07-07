/*******************************************\
  游戏人客户端分页导航类(2006-7-26)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.Pager = function (cName)
{
	this.Total = 0;
	this.PageSize = 0;
	this.CurrentPage = 1;
	this.PageButtonNumber = 5;
	this.ShowPagerBar = function ()
	{
		this.PageCount = Math.ceil(this.Total/this.PageSize);
		this.CurrentPage = this.CurrentPage * 1;
		if (this.CurrentPage>this.PageCount) this.CurrentPage=this.PageCount;
		if (this.CurrentPage<1) this.CurrentPage=1;
		if (this.PageButtonNumber%2==0) this.PageButtonNumber++
		this.PageButtonNumber2 = (this.PageButtonNumber - 1) / 2;
		var Html;
		if (this.CurrentPage==1) Html = "[上一页] ";
		else Html = "<a href=\"javascript:" +cName+ ".goPrePage()\">[上一页]</a> ";
		//分三种情况
		if (this.CurrentPage<=this.PageButtonNumber2)
			for (var i=1; i<=this.PageButtonNumber; i++)
			{
				if (i>this.PageCount) break;
				if (i==this.CurrentPage)
				Html += "[" +i+ "] ";
				else
				Html += "<a href=\"javascript:" +cName+ ".goPage(" +i+ ")\">[" +i+ "]</a> ";
			}
		else if (this.CurrentPage+this.PageButtonNumber2>this.PageCount)
			for (var i=this.PageCount-this.PageButtonNumber+1; i<=this.PageCount; i++)
			{
				if (i>this.PageCount) break;
				if (i==this.CurrentPage)
				Html += "[" +i+ "] ";
				else
				Html += "<a href=\"javascript:" +cName+ ".goPage(" +i+ ")\">[" +i+ "]</a> ";
			}
		else
			for (var i=this.CurrentPage-this.PageButtonNumber2; i<=this.CurrentPage+this.PageButtonNumber2; i++)
			{
				if (i>this.PageCount) break;
				if (i==this.CurrentPage)
				Html += "[" +i+ "] ";
				else
				Html += "<a href=\"javascript:" +cName+ ".goPage(" +i+ ")\">[" +i+ "]</a> ";
			}
		if (this.CurrentPage==this.PageCount) Html+= "[下一页] ";
		else Html += "<a href=\"javascript:" +cName+ ".goNexPage()\">[下一页]</a> ";
		Html += "[" +this.CurrentPage+ "/" +this.PageCount+ "] "
		Html += "<input type=\"text\" size=\"3\" id=\"_DronFw_Pager_PagerInput\"><input type=\"button\" value=\"Go!\" onclick=\"" +cName+ ".goPage(document.getElementById(\'_DronFw_Pager_PagerInput\').value)\">";
		return Html;
	}
}