/*******************************************\
  游戏人滚动字幕类(2006-06-16)
  This JavaScript was writen by Dron.
  @2003-2008 Ucren.com All rights reserved.
\*******************************************/
DronFw.Class.ScrollBox = function (uid)
{
	this.scrollBox = document.getElementById(uid);
	this.scrollBoxHeight = this.scrollBox.clientHeight;
	this.scrollBoxInner = this.scrollBox.innerHTML;
	this.scrollCol = this.scrolln = 0;
	this.setScroll = function ()
	{
		this.scrollBox.scrollTop = this.scrollCol + this.scrolln;
		if(this.scrolln==this.scrollBoxHeight)
			return this.addScroll()
		else
			this.scrolln ++;
		var o = this;
		function m(){o.setScroll();}
		setTimeout(m,20);
	}
	this.addScroll = function ()
	{
		this.scrollBox.innerHTML += "<br>" + this.scrollBoxInner;
		this.scrollCol = this.scrollBox.scrollTop;
		this.scrolln = 0;
		var o = this;
		function m(){o.setScroll();}
		setTimeout(m,1000);
	}
	this.addScroll();
}