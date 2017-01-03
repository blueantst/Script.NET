var maxthon_maps = {

	maps: null,
	logo: null,

	maps_data:
	{
		'0': {'name': '傲游官方网站', 'depth': 0, 'url': 'http://cn.maxthon.com/', 'logo': 'http://www.maxthon.cn/extend/logo_menu/images/m1_logo.png'},
		'1': {'name': '傲游2.0下载试用', 'depth': 0, 'url': 'http://www.maxthon.cn/', 'logo': 'http://www.maxthon.cn/extend/logo_menu/images/m2_logo.png'},
		'2': {'name': '傲游论坛', 'depth': 0, 'url': 'http://forum.maxthon.cn/', 'logo': 'http://www.maxthon.cn/extend/logo_menu/images/forum_logo.png'},
		'3': {'name': '傲游扩展', 'depth': 0, 'url': 'http://addons.maxthon.cn/', 'logo': 'http://www.maxthon.cn/extend/logo_menu/images/addons_logo.png'},
		'4': {'name': '傲游志', 'depth': 0, 'url': 'http://blogcn.maxthon.com/', 'logo': 'http://www.maxthon.cn/extend/logo_menu/images/blog_logo.png'},
		'5': {'name': '傲游招聘', 'depth': 0, 'url': 'http://hr.maxthon.com/', 'logo': 'http://www.maxthon.cn/extend/logo_menu/images/hr_logo.png'},
		'6': {'name': '傲游帐户', 'depth': 0, 'url': 'http://passport.maxthon.cn/', 'logo': 'http://www.maxthon.cn/extend/logo_menu/images/passport_logo.png'}
	},
	
	//return the top and left of a element by id
	get_location: function(id)
	{
		var obj = document.getElementById(id);
		var left = obj.offsetLeft;
		var top = obj.offsetTop;
		var objParent = obj.offsetParent;
		
		while(objParent.tagName.toUpperCase()!= "BODY"){
			left += objParent.offsetLeft;
			top += objParent.offsetTop;
			objParent = objParent.offsetParent;
		}
		return {'x': left, 'y': top};
	},

	//produce the web maps when beginning
	produce_maps: function()
	{
		this.logo = document.getElementById('maxthon_explorer_logo');
		if (!this.logo) return;
		var root = document.createElement('ul');
		root.id = 'maxthon_explorer_maps';
		root.style['position'] = 'absolute';
		root.style['display'] = 'none';
		root.style['backgroundColor'] = 'white';
		var temp, i, item;
		for (i in this.maps_data)
		{
			item = this.maps_data[i];
			temp = document.createElement('li');
			temp.innerHTML = '<a href="' + item.url + '">' +
				'<img alt="" src="' + item.logo + '" />' +			
				'<span>' + item.name + '</span></a>';
			root.appendChild(temp);
		}
		document.getElementsByTagName('body')[0].appendChild(root);
		this.maps = document.getElementById('maxthon_explorer_maps');
		if (window.addEventListener)
		{
			maxthon_maps.logo.addEventListener('mouseover', function(){maxthon_maps.show_maps();}, false);
			maxthon_maps.maps.addEventListener('mouseover', function(){maxthon_maps.show_maps();}, false);
			maxthon_maps.logo.addEventListener('mouseout', function(){maxthon_maps.hide_maps();}, false);
			maxthon_maps.maps.addEventListener('mouseout', function(){maxthon_maps.hide_maps();}, false);
		}
		else if (window.attachEvent)
		{
			maxthon_maps.logo.attachEvent('onmouseover', function(){maxthon_maps.show_maps();});
			maxthon_maps.maps.attachEvent('onmouseover', function(){maxthon_maps.show_maps();});
			maxthon_maps.logo.attachEvent('onmouseout', function(){maxthon_maps.hide_maps();});
			maxthon_maps.maps.attachEvent('onmouseout', function(){maxthon_maps.hide_maps();});
		}
	},

	hide_maps: function ()
	{
		this.maps.style['display'] = 'none';
	},
	
	show_maps: function ()
	{
		var location = this.get_location('maxthon_explorer_logo');
		this.maps.style['left'] = location.x + 25 + "px";
		this.maps.style['top'] = location.y + 25 + "px";
		this.maps.style.display = 'block';
	}
}

// Execution
maxthon_maps.produce_maps();
