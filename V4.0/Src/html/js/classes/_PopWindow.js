/*******************************************\
  游戏人popup窗体类 For IE5.5+ (2006-8-23)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.PopWindow = function ()
{
	if (!/msie/i.test(navigator.userAgent)) return window.alert("IE only!");
	var source;
	var me = this;
	var pwin = window.createPopup();
	this.StyleName = "metal";
	this.Left = 0;
	this.Top = 0;
	this.Width = 300;
	this.Height = 200;
	this.Ico = DronFw.Path + "classes/_PopWindow/ico.gif";
	this.Title = "Undefine Title";
	this.Content = "&nbsp;";
	this.MinButton = true;
	this.MaxButton = true;
	this.CloButton = true;
	this.OnTop = true;

	this.ecaLeft = this.ecaTop = 0;
	this.draging = false;
	this.maxed = false;

	this.url = "about:blank";

	this.Open = function (url)
	{
		function getTextByFile(file)
		{
			var h = new ActiveXObject("Microsoft.XMLHTTP");
			var r = new ActiveXObject("ADODB.RecordSet");
			h.open("get", file, false);
			try      { h.send(null);}
			catch(e) { return false;};
			if (h.status==200 || h.status==0)
			{
				r.Fields.Append("DDD",201,1);
				r.Open();
				r.AddNew();
				r(0).AppendChunk(h.responseBody);
				r.Update();
				var re = r(0).value;
				r.Close();
				return re;
			}
			else return false;
		}
		if (typeof (url)=="string") source = getTextByFile(url);
	}
	this.SetContent = function (content)
	{
		source = content;
	}
	this.SetDivId = function(id)
	{
	}
	this.Show = function (obj)
	{
		var eve = " onmouseover=\"this.className=(this.className.substr(0,3)+\'on\')\" onmouseout=\"this.className=this.className.substr(0,3)\" onmousedown=\"this.className=this.className.substr(0,3)+\'down\'\" onmouseup=\"this.className=this.className.substr(0,3)+\'on\'\"";
		var HTML = "<html>\
			<head>\
			<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">\
			<meta http-equiv=\"Content-Language\" content=\"zh-CN\">\
			<META HTTP-EQUIV=\"MSThemeCompatible\" CONTENT=\"Yes\">\
			<link id=\"stylelink\" rel=\"stylesheet\" href=\"" + DronFw.Path + "classes/_PopWindow/" +me.StyleName+ "/pwin.css\" type=\"text/css\" media=\"all\">\
			<title>" + this.Title + "</title>\
			</head>\
			<body>\
				<table id=\"MainTable\" cellspacing=\"0\" cellpadding=\"0\">\
					<tr>\
						<td class=\"lt\"></td>\
						<td class=\"t\" " + (me.MaxButton?"ondblclick=\"parent." +obj+ ".maxIt();\"":"") + " onmousedown=\"parent." +obj+ ".drag(event)\" onmouseup=\"parent." +obj+ ".drop(event)\" onmousemove=\"parent." +obj+ ".move(event)\">\
							<div class=\"ico\" style=\"background-image:url(" + this.Ico + ")\">&nbsp;</div>\
							<div class=\"title\">&nbsp;" + me.Title + "<div class=\"title_shadown\">&nbsp;" + me.Title + "</div></div>";
		HTML += me.CloButton ? "<div class=\"bt3\"" +eve+ " onclick=\"parent." +obj+ ".closeIt();\">&nbsp;</div>" : "";
		HTML += me.MaxButton ? "<div class=\"bt2\"" +eve+ " onclick=\"parent." +obj+ ".maxIt()\">&nbsp;</div>" : "";
		HTML += me.MinButton ? "<div class=\"bt1\"" +eve+ " onclick=\"parent.setTimeout(parent." +obj+ ".reShow,1);parent." +obj+ ".maxed=false;parent.alert(\'此功能未完成，请稍候...\');\">&nbsp;</div>" : "";
		HTML += "		</td>\
						<td class=\"rt\"></td>\
					</tr>\
					<tr>\
						<td class=\"lr\"></td>\
						<td class=\"m\"><div id=\"mainFrame\">" +(source ? source : this.Content)+ "</div></td>\
						<td class=\"lr\"></td>\
					</tr>\
					<tr>\
						<td class=\"lb\"></td>\
						<td class=\"b\"></td>\
						<td class=\"rb\"></td>\
					</tr>\
				</table>\
			</body>\
			</html>";
		pwin.document.open();
		pwin.document.write(HTML);
		pwin.document.close();
		pwin.show(me.Left, me.Top, me.Width, me.Height);
		this.Window = pwin;
		if (me.OnTop) window.onblur = me.reShow;
		document.body.attachEvent("onmousedown",function ()
		{
			me.reShow();
		});
	}
	this.reShow = function ()
	{
		if (me.closed) return;
		pwin.show(me.Left, me.Top, me.Width, me.Height);
		me.maxed = false;
	}
	this.drag = function (e)
	{
		if (me.maxed) return;
		me.ecaLeft = e.screenX - me.Left;
		me.ecaTop = e.screenY - me.Top;
		me.draging = true;
		e.srcElement.setCapture();
	}
	this.drop = function (e)
	{
		me.draging = false;
		e.srcElement.releaseCapture();
	}
	this.move = function (e)
	{
		if (me.draging)
		{
			me.Left = e.screenX - me.ecaLeft;
			me.Top = e.screenY - me.ecaTop;
			if (me.Top<window.screenTop) me.Top = window.screenTop;
			if (me.Left<window.screenLeft-me.Width) me.Left = window.screenLeft-me.Width;
			if (me.Top>window.screenTop+document.body.offsetHeight-me.Height) me.Top=window.screenTop+document.body.offsetHeight-me.Height;
			if (me.Left>window.screenLeft+document.body.offsetWidth) me.Left=window.screenLeft+document.body.offsetWidth;
			me.reShow();
		}
	}
	this.maxIt = function ()
	{
		if (me.maxed) return me.reShow();
		else pwin.show(0, 0, screen.availWidth, screen.availHeight);
		me.maxed = true;
	}
	this.closeIt = function ()
	{
		pwin.hide();
		me.closed = true;
	}
	this.SetStyle = function (sn)
	{
		me.StyleName = sn;
		pwin.document.getElementById("stylelink").setAttribute("href",DronFw.Path + "classes/_PopWindow/" +me.StyleName+ "/pwin.css")
	}
};