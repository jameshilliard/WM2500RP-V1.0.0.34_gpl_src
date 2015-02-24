function keydown(e,text)
{
        if((e.keyCode == 190 || e.keyCode == 110) && text.value.length !=0 && SelectionTextLength(text)==0)
                text.form[(getIndex(text)+1) % text.form.length].focus();
}
function keyup(e,text)
{
        if(text.value.length == 3 && ((e.keyCode >47 && e.keyCode <58) ||(e.keyCode >95 && e.keyCode <106)))
                text.form[(getIndex(text)+1) % text.form.length].focus();
}
function keyup_n(e,text,num)
{
        if(text.value.length == num && (e.keyCode != 190 && e.keyCode != 110 && e.keyCode != 9))
                text.form[(getIndex(text)+1) % text.form.length].focus();
}

function isValidNumLetter(each_char)
{
        if((each_char>47 && each_char<58)||(each_char>64 && each_char<91)||(each_char>96 && each_char<123))
                return true;
        else
                return false;
}

function isValidLetter(each_char)
{
        if((each_char>64 && each_char<91)||(each_char>96 && each_char<123))
                return true;
        else
                return false;
}

function change_ipv6(type)
{
        switch(type)
        {
                case "disabled":
                        location.href="IPv6_disabled.htm";
                        break;
                case "autoDetect":
                        location.href="IPv6_auto.htm";
                        break;
                case "autoConfig":
                        location.href="IPv6_autoConfig.htm";
                        break;
                case "6to4":
                        location.href="IPv6_tunnel.htm";
                        break;
                case "bridge":
                        location.href="IPv6_passThrougth.htm";
                        break;
                case "fixed":
                        location.href="IPv6_fixed.htm";
                        break;
                case "dhcp":
                        location.href="IPv6_dhcp.htm";
                        break;
                case "pppoe":
                        location.href="IPv6_pppoe.htm";
                        break;
                default:
                        location.href="IPv6_disabled.htm";
                        break;
        }
}

function address_parseInt(addr)/*to fix bug 26258*/
{
	if( "" == addr )
		return addr;

	addr_array = addr.split(".");
	for(i=0; i< 4; i++)
	{
		if( "" == addr_array[i] )
			addr_array[i] = 0;
		if( 0 == i )
			addr = parseInt(addr_array[i], 10);
		else
			addr = addr + "." +parseInt(addr_array[i], 10);
	}
	return addr;
}

String.prototype.checkNum=function()
{// check string is num or not, if have other characters return true
	var s = "1234567890";
	for( var i=0; i<this.length; i++){
		if(s.indexOf(this.charAt(i)) == -1)
			return true;
	}

	return false;
}

function IE_version()
{
	var Sys = {};
	var ua = navigator.userAgent.toLowerCase();
	var s;
	(s = ua.match(/msie ([\d.]+)/)) ? Sys.ie = s[1] :0;
	if(Sys.ie == '6.0')
		return 6;
	else if(Sys.ie == '7.0')
		return 7;
	else if(Sys.ie == '8.0')
		return 8;
	else if(Sys.ie == '9.0')
		return 9;
	else
		return false;
}

function delete_end_space(str) //delete all the space at the end of a sting.
{
        var i;

        for(i=str.length-1; str.charAt(i)==" "; i--)
        {
                str = str.substring(0, i);
        }

        return str;
}

function remove_space(str)
{
        str = delete_end_space(str);
        str = str.replace(/^\s*/, "");

        return str;
}

function IE_RST_status()
{
	if( isIE_or_Opera() && IE_version()!=9 )
	{
		var frame_height= top.document.getElementById("formframe_div").style.height.replace(/px/,"");
		document.getElementById("content_icons").style.height= frame_height;
	}
}

function mac_os_popup()
{
	if(navigator.userAgent.indexOf("Mac OS") != -1)
	{
		if(get_browser() == "")
			return "popup";
		else
			return "";
	}
	else
		return "";
}

function change_blank(str)
{
	var test_str="";
	j = str.lastIndexOf("&nbsp;");
	for(k=0;k<str.length;k++) 
	{
		if(k == j)
		{
			test_str+=" ";
			k=k+5;
		} 
		else
		  test_str+=str.substring(k,k+1);
	}
	return test_str;
}

//tzo:0-Africa;1-Asia;2-Australia;3-Canada;4-Europe;5-Israel;6-Japan
//7-Korea;8-Mexico;9-South America;10-United States;11-Middle East;
function check_time_zone(tzo,reg,firm_reg)
{
	reg = reg.substr(0,2);
	if(firm_reg == "NA")
		return 10;
	else
	{
		if(tzo == "-8" || tzo == "-7" || tzo == "-6")
		{
			if(reg.indexOf('fr') > -1 || reg.indexOf('en') > -1)
				return 3;
			else
				return 4;
		}
		else if(tzo == "-5")
		{
			if(reg.indexOf('fr') > -1 || reg.indexOf('en') > -1)
				return 3;
			else if(reg.indexOf('es') > -1)
				return 8;
			else
				return 4;
		}
		else if(tzo == "-4")
		{
			if(reg.indexOf('fr') > -1 || reg.indexOf('en') > -1)
				return 3;
			else
				return 9;
		}
		else if(tzo == "-3")
		{
			if(reg.indexOf('fr') > -1 || reg.indexOf('en') > -1)
				return 3;
			else if(reg.indexOf('es') > -1 || reg.indexOf('pt') > -1)
				return 9;
			else
				return 4;
		}
		else if(tzo == "2")
		{
			if(reg.indexOf('en') > -1 || reg.indexOf('af') > -1)
				return 0;
			else if(reg.indexOf('ar') > -1)
				return 11;
			else
				return 4;
		}
		else if(tzo == "3")
		{
			if(reg.indexOf('en') > -1)
				return 5;
			else if(reg.indexOf('ar') > -1)
				return 11;
			else
				return 4;
		}
		else if(tzo == "4")
		{
			if(reg.indexOf('ar') > -1)
				return 11;
			else
				return 1;
		}
		else if(tzo == "8")
		{
			if(reg.indexOf('en') > -1)
				return 2;
			else
				return 1;
		}
		else if(tzo == "9")
		{
			if(reg.indexOf('ko') > -1)
				return 7;
			else if(reg.indexOf('en') > -1)
				return 2;
			else if(reg.indexOf('ja') > -1)
				return 6;
			else
				return 4;
		}
		else if(tzo == "10" && reg.indexOf('en') > -1)
			return 2;
		else if(tzo == "0" || tzo == "1")
			return 4;
		else if(tzo == "5" || tzo == "6" || tzo == "7")
			return 1;
		else
			return 4;
	}
}

function check_mobile_device()
{
	if(navigator.userAgent.indexOf("iPhone") != -1)
		return 1;
	else if(navigator.userAgent.indexOf("iPad") != -1 )
		return 0;
	else if(window.screen.width <= 960)
		return 1;
	else
		return 0;
}

function check_gui_region(reg)
{
	if(reg == "Korean" || reg == "Japanese" || reg == "Chinese")
		return 1;
	else
		return 0;
}

function xss_replace(xss_str)
{
	xss_str = xss_str.replace(/&#38;/g,'&').replace(/&#35;/g,'#').replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&gt;/g,'>').replace(/&lt;/g,'<').replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&nbsp;/g," ");
	return xss_str;
}

function Not_IE_show_hidden_help(help_flag)
{
	if( help_flag % 2 == 0 )
	{
		document.getElementById("main").style.bottom="150px";
		document.getElementById("help").style.display="";
		document.getElementById("help_switch").className="open_help";
	}
	else
	{
		document.getElementById("main").style.bottom="30px";
		document.getElementById("help").style.display="none";
		document.getElementById("help_switch").className="close_help";
	}
}

function show_hidden_help(help_flag)
{
	if(!isIE_or_Opera())
		Not_IE_show_hidden_help(help_flag);
	else
	{
		var frame_height= top.document.getElementById("formframe_div").style.height.replace(/px/,"");
		if( help_flag % 2 == 0 )
		{
			document.getElementById("main").style.height=frame_height-170;//20+30+120
			document.getElementById("help").style.display="";
			document.getElementById("help_switch").className="open_help";
		}
		else
		{
			document.getElementById("help").style.display="none";
			document.getElementById("help_switch").className="close_help";
			document.getElementById("main").style.height=frame_height-50;//20+30
		}
	}
}

function show_hidden_help_top_button(help_flag)
{
	if(!isIE_or_Opera())
		Not_IE_show_hidden_help(help_flag);

	else
	{
		var frame_height= top.document.getElementById("formframe_div").style.height.replace(/px/,"");
		if( help_flag % 2 == 0 )
		{
			document.getElementById("main").style.height=frame_height-200;//50+30+120
			document.getElementById("help").style.display="";
			document.getElementById("help_switch").className="open_help";
		}
		else
		{
			document.getElementById("help").style.display="none";
			document.getElementById("help_switch").className="close_help";
			document.getElementById("main").style.height=frame_height-80;//50+30
		}
	}
}

function get_browser()
{
	if(navigator.userAgent.indexOf("MSIE") != -1)
		return "IE";
	else if(navigator.userAgent.indexOf("Chrome") != -1 )
		return "Chrome";//bug 21975:spec1.9-p228,[usb] the real links are different for different browsers
	else if(navigator.userAgent.indexOf("Firefox") != -1)
		return "Firefox";
	else if(navigator.userAgent.indexOf("Safari") != -1 )
		return "Safari";
	else if(navigator.userAgent.indexOf("Camino") != -1) 
		return "Camino"; 
 	else if(navigator.userAgent.indexOf("Gecko/") != -1)
   		return "Gecko"; 
	else if(navigator.userAgent.indexOf("Opera") != -1)
		return "Opera";
	else
   		return "";		
}

function isIE_or_Opera()
{
	var browser = get_browser();
	if( browser == "IE" || browser == "Opera" )
		return true;
	else
		return false;
}

function change_height()
{
	var footer_div = document.getElementById("footer");
	var is_double = ( footer_div.className == "footer_double") ;

	if(isIE_or_Opera())
	{
		var height = document.documentElement.clientHeight - 97;
		if( (height > 460 && !is_double) || (height > 503 && is_double) )
		{
			document.getElementById("container").style.height = height;
			document.getElementById("middle").style.height = height-5;
			document.getElementById("menu").style.height = is_double ? height-93 : height-50;
			document.getElementById("formframe_div").style.height = is_double ? height-93 : height-50;
		}
		else
		{
			document.getElementById("container").style.height = is_double ? "503px" : "460px";
			document.getElementById("middle").style.height = is_double ? "498px" : "455px";
			document.getElementById("menu").style.height = "410px";
			document.getElementById("formframe_div").style.height = "410px";
		}
	}
	document.getElementById("middle").style.minHeight = is_double ? "498px" : "455px";
	document.getElementById("menu").style.bottom = is_double ? "88px" : "45px";
	document.getElementById("formframe_div").style.bottom = is_double ? "88px" : "45px";
}

/*function change_height()
{
	var footer_div = document.getElementById("footer");
	var is_double = ( footer_div.className == "footer_double") ;
	var menu_height = document.getElementById("menu").clientHeight > 410 ? document.getElementById("menu").clientHeight : 410;
	var sUserAgent = navigator.userAgent;
	if(sUserAgent.indexOf("MSIE 9.0") > -1)
		document.getElementById("middle").style.minHeight = is_double ? (menu_height + 87)+"px" : (menu_height+ 45)+"px";
	else if(isIE_or_Opera() && IE_version() !=9)
	{
		var height = is_double ? document.documentElement.clientHeight - 190 : document.documentElement.clientHeight - 147;
		menu_height = height > menu_height ? height : menu_height;
		document.getElementById("container").style.height = is_double ? menu_height+93 : menu_height+ 50;
		document.getElementById("middle").style.height = is_double ?  menu_height+87 : menu_height+ 45;
		document.getElementById("formframe_div").style.height = menu_height;
	}
	else
		document.getElementById("middle").style.minHeight = is_double ? (menu_height + 87)+"px" : (menu_height+ 45)+"px";
}*/

function change_wizard_height()
{
	if(isIE_or_Opera())
	{
		var height = document.documentElement.clientHeight - document.body.scrollHeight;
		if( height > 460 ) 
		{
			document.getElementById("container").style.height = height;
			document.getElementById("wizard_middle").style.height = height-5;
			/*if( gui_region == "German")
				document.getElementById("wizard_menu_small").style.height = height-50;
			else*/
				document.getElementById("wizard_menu").style.height = height-50;
			document.getElementById("wizard_formframe_div").style.height = height-50;
		}
		else
		{
			document.getElementById("container").style.height = "460px";
			document.getElementById("wizard_middle").style.height = "455px";
			/*if( gui_region == "German")
				document.getElementById("wizard_menu_small").style.height = "410px";
			else*/
				document.getElementById("wizard_menu").style.height = "410px";
			document.getElementById("wizard_formframe_div").style.height = "410px";
		}
	}
	document.getElementById("wizard_middle").style.minHeight = "455px";
	/*if( gui_region == "German")
		document.getElementById("wizard_menu_small").style.bottom = "45px";
	else*/
		document.getElementById("wizard_menu").style.bottom = "45px";
	document.getElementById("wizard_formframe_div").style.bottom = "45px";
}

function setFooterClass()
{
	var footer_div = top.document.getElementById("footer");
	var content = footer_div.innerHTML.replace(/<\/?.+?>/g,"").replace(/[\r\n]/g, "").replace(/\s+/g, "");
	var content_len = content.length;

	if(content_len > 75)
		footer_div.className = "footer_double";
	else
		footer_div.className = "footer";
	
	var go_btn = top.document.getElementById("search_button");
	content_len = go_btn.value.length;
	
	if(content_len >= 7)
		go_btn.className = go_btn.className + "_long";
	else if(content_len >= 4)
		go_btn.className = go_btn.className + "_middle";
	else 
		go_btn.className = go_btn.className;
}

function close_all_sub(click_name)/* fold all menus, except the menu which user click */
{
	var sub_list = "setup,admin,advanced";
	var sub_array = sub_list.split(',');
	for ( i=0; i< sub_array.length; i++)
	{
		var button_name = sub_array[i]+"_bt";
		var sub_name = sub_array[i]+"_sub";
		if( sub_name != click_name )
		{
			var div_name = top.document.getElementById(button_name);
			var content_length = div_name.getElementsByTagName("span")[0].innerHTML.length;
			settingClass(div_name, content_length, "advanced_white_close_button", top.region_class.white_triple, top.region_class.white_double);

			top.document.getElementById(sub_name).style.display="none";
		}
	}
}

function open_or_close_sub(name)
{
	/* to fix bug 23268, when user want to unfold one menus, fold the other menus. */
	var button_name= name+"_bt";
	var sub_name= name+"_sub";
	var open_flag= top.document.getElementById(sub_name).style.display;
	
	close_all_sub(sub_name);/* fold all menus first, except the menu which user click*/

	var button_div = top.document.getElementById(button_name);
	var content_length = button_div.getElementsByTagName("span")[0].innerHTML.length;
	if( open_flag == "none")
	{
		settingClass(button_div, content_length, "advanced_white_open_button", top.region_class.white_triple, top.region_class.white_double);
		top.document.getElementById(sub_name).style.display="";
	}
	else
	{
		settingClass(button_div, content_length, "advanced_white_close_button", top.region_class.white_triple, top.region_class.white_double);
		top.document.getElementById(sub_name).style.display="none";
	}
	//change_menu_height();
}

/* change the min-height of the fromframe if unfold "Advanced Setup" */
function change_menu_height()
{
	var footer_div = document.getElementById("footer");
	var is_double = (footer_div.className == "footer_double");

	if(top.document.getElementById("advanced_sub"))
	{
	   if( top.document.getElementById("advanced_sub").style.display == "none" )
	   {
		if(isIE_or_Opera())
		{
			var height = document.documentElement.clientHeight - document.body.scrollHeight;
			if( (height <= 460 && !is_double) || (height <= 590 && is_double))
			{
				document.getElementById("container").style.height = is_double ? "590px" : "460px";
				document.getElementById("middle").style.height = is_double ? "585px" : "455px";
				document.getElementById("menu").style.height = is_double ? "487px" : "410px";
				document.getElementById("formframe_div").style.height = is_double ? "497px" : "410px";
			}
			else
			{
				document.getElementById("container").style.height=height;
				document.getElementById("middle").style.height=height-5;
				document.getElementById("menu").style.height=is_double?height-93:height-50;
				document.getElementById("formframe_div").style.height=is_double?height-93:height-50;
			}
		}
		else
			top.document.getElementById("middle").style.minHeight = is_double ? "573px" : "455px";
	   }
	   else
	   {
		if(isIE_or_Opera())
		{
			var height = document.documentElement.clientHeight - document.body.scrollHeight;
			if( (height <= 555  && !is_double) || ( height <= 700 && is_double) )
			{
				document.getElementById("container").style.height = is_double ? "700px" : "555px";
				document.getElementById("middle").style.height = is_double ? "695px" : "550px";
				document.getElementById("menu").style.height = is_double ? "607px" : "505px";
				document.getElementById("formframe_div").style.height = is_double ? "607px" : "505px";
			}
			else
			{
				document.getElementById("container").style.height=height;
				document.getElementById("middle").style.height=height-5;
				document.getElementById("menu").style.height=is_double?height-93:height-50;
				document.getElementById("formframe_div").style.height=is_double?height-93:height-50;
			}
		}
		else
			top.document.getElementById("middle").style.minHeight = is_double ? "710px" : "550px";
	   }
	}
}

/* according to the content length in a div,  change the div class type 
   parameter: div name, div content length, class name to set, minimum length for triple class, .. 
*/
function settingClass(obj, len, class_name)
{
	var triple_len, double_len;
	switch(arguments.length)
	{
		case 4://four parameter, take the last on as double_len 
			triple_len = 9999;
			double_len = arguments[3];
			break;
		case 5:
			triple_len = arguments[3];
			double_len = arguments[4];
			break;
		default:
			triple_len = top.region_class.adv_btn_triple;
			double_len = top.region_class.adv_btn_double;
			break;
	}
	if((gui_region == "French" || gui_region == "German") && IE_version() == 9)
		double_len = 21;
	if(gui_region == "Russian" && IE_version() == 9)
		double_len = 20;
	if(gui_region == "Italian" && IE_version() == 9)
		double_len = 19;
		
	if(len >= triple_len)
		obj.className = class_name + "_triple";
	else if(len >= double_len)
		obj.className = class_name + "_double";
	else
		obj.className = class_name;
}

function subItemsClass(argv)
{
	var sub_menu;
	var items;
	var words_length;
	var i, num;

	for(num=0; num<arguments.length; num++)
	{
		sub_menu = top.document.getElementById(arguments[num]);
		items = sub_menu.getElementsByTagName("dt");

		for(i=0; i<items.length; i++)
		{
			words_length = items[i].getElementsByTagName("span")[0].innerHTML.length;
			if(words_length >= 20)
				items[i].className = "long_name";
			else
				items[i].className = "sub_back";
		}
	}
}

var array_name = ["ap_items"];
var enable_flags = ["enable_ap_flag"];
var ap_items = ["wireless", "bas_ether", "coax", "status", "attached", "bak_set", "passwd", "upgrade", "wladv", "vlan", "ipv6"];
function enabledItemsClass()
{
	var i, j;
	var double_length = top.region_class.sub_double;
	
	for(i=0; i<array_name.length; i++)
	{
		var item_group, enable_flag;

		item_group = eval(array_name[i]);
		enable_flag = eval("top." + enable_flags[i]);
		for(j=0; j<item_group.length; j++)
		{
			var cur_div, content_length;

			cur_div = top.document.getElementById(item_group[j]);
			content_length = cur_div.getElementsByTagName("span")[0].innerHTML.length;
			if(enable_flag == 1)
			{
				if(content_length > double_length)
					cur_div.className = "long_grey";
				else
					cur_div.className = "sub_grey";
			}
			else
			{
				if(content_length > double_length)
					cur_div.className = "long_name";
				else
					cur_div.className = "sub_back";
			}
		}
	}
}

function menu_class_default()
{
	var menu_div;
	var content_length;

	menu_div = top.document.getElementById("setup_wizard");
	content_length = menu_div.getElementsByTagName("span")[0].innerHTML.length;
	settingClass(menu_div, content_length, "advanced_black_button");

	menu_div = top.document.getElementById("wps");
	content_length = menu_div.getElementsByTagName("span")[0].innerHTML.length;
	if ( top.ap_mode==0 || top.enabled_wps == 0)
		settingClass(menu_div, content_length, "advanced_grey_button");
	else
		settingClass(menu_div, content_length, "advanced_black_button");
	
        var extensible_items = ["setup_bt", "admin_bt", "advanced_bt"];
	var i;
	for(i=0; i<extensible_items.length; i++)
	{
		menu_div = top.document.getElementById(extensible_items[i]);
		content_length = menu_div.getElementsByTagName("span")[0].innerHTML.length;
		settingClass(menu_div, content_length, "advanced_white_close_button", top.region_class.white_triple, top.region_class.white_double);
	}

	subItemsClass("setup_sub", "admin_sub", "advanced_sub");

	enabledItemsClass();
}

function menu_color_change( change_id )
{
	menu_class_default();

	var current_div = top.document.getElementById(change_id);
	var content_length = current_div.getElementsByTagName("span")[0].innerHTML.length;
	
	if( change_id == "setup_wizard" || change_id == "wps" )
	{
		settingClass(current_div, content_length, "advanced_purple_button");
	}
	else
	{
		//the parent button class should be kept
		var parent_id = top.document.getElementById(change_id).parentNode.parentNode.id;
		var btn_id = parent_id.replace('sub', 'bt');
		var btn_div = top.document.getElementById(btn_id);
		var words_len = btn_div.getElementsByTagName("span")[0].innerHTML.length;
		settingClass(btn_div, words_len, "advanced_white_open_button", top.region_class.white_triple, top.region_class.white_double);

		content_length = current_div.getElementsByTagName("span")[0].innerHTML.length;
		settingClass(current_div, content_length, "sub_back_purple", top.region_class.sub_triple, top.region_class.sub_double);
	}
}

function goto_formframe(page)
{
	top.formframe.location.href=page;
}

function click_action(id)
{
	if( enable_action == 1 )
    {
		if(id == "setup_wizard")
		{
			menu_color_change('setup_wizard');
			if(con_status!="1")
				location.href='wizard_to_welcome.htm';
			else
				goto_formframe('wizard_already.htm');
		}
		else if( id == "wps" && top.ap_mode!=0 && top.enabled_wps!=0)
		{
			goto_formframe('add_wps_client.htm');
			menu_color_change('wps');
		}
		else if( id == "internet" )
		{
			goto_formframe('wait_for_searching.htm');
			menu_color_change('internet');
		}
		else if( id == "wireless" )
		{
			goto_formframe('WLG_wireless.htm');
			menu_color_change('wireless');
		}
		else if( id == "guest" )
		{
			goto_formframe('WLG_wireless_guest1.htm');
			menu_color_change('guest');
		}
		else if( id == "bas_ether" )
		{
			goto_formframe('bas_ether.htm');
			menu_color_change('bas_ether');
		}
		else if( id == "coax" )
		{
			goto_formframe('coax.htm');
			menu_color_change('coax');
		}
		else if( id == "status" )
		{
			goto_formframe('RST_status.htm');
			menu_color_change('status');
		}
		else if( id == "attached" )
		{
			goto_formframe('attach_device_wait.htm');
			menu_color_change('attached');
		}
		else if( id == "bak_set" )
		{
			goto_formframe('backup_settings.htm');
			menu_color_change('bak_set');
		}
		else if( id == "passwd" )
		{
			goto_formframe('passwd.htm');
			menu_color_change('passwd');
		}
		else if( id == "upgrade" )
		{
			goto_formframe('upgrade.htm');
			menu_color_change('upgrade');
		}
		else if( id == "wladv" )
		{
			goto_formframe('WLG_adv.htm');
			menu_color_change('wladv');
		}
		else if( id == "vlan" )
		{
			goto_formframe('WLG_vlan.htm');
                        menu_color_change('vlan');
		}
		else if( id == "ipv6" )
		{
			goto_formframe('IPv6_index.htm');
			menu_color_change('ipv6');
		}
		else if( id == "tr069" )
		{
			goto_formframe("tr069.htm");
			menu_color_change('tr069');
		}
		else if( id == "snmp" )
		{
			goto_formframe("snmp.htm");
			menu_color_change('snmp');
		}
	}
}

function is_sub_or_broad(be_checkip, ip, mask)
{
        addr_arr = be_checkip.split('.');
        var ip_addr=0;
        for (i = 0; i < 4; i++)
        {
                addr_str = parseInt(addr_arr[i],10);
                ip_addr=ip_addr*256+parseInt(addr_str);
        }
        var ip_sub=isSub(ip, mask);
        var ip_broadcast=isBroadcast(ip, mask)
	if(ip_addr == ip_sub || ip_addr == ip_broadcast){
		return false;
	}
	return true;
}

function time_stp()
{
				document.write('<input type="hidden" name="time_stamp_dni" value="'+time_stamp_dni+'">');
				parent.index_ts = time_stamp_dni;	
}

function get_browser()
{
        if(navigator.userAgent.indexOf("MSIE") != -1)
                return "IE";
        else if(navigator.userAgent.indexOf("Firefox") != -1)
                return "Firefox";
	else if(navigator.userAgent.indexOf("Chrome") != -1 )
                return "Chrome";		
        else if(navigator.userAgent.indexOf("Safari") != -1 )
                return "Safari";
        else if(navigator.userAgent.indexOf("Camino") != -1)
                return "Camino";
        else if(navigator.userAgent.indexOf("Gecko/") != -1)
                return "Gecko";
        else
                return "";
}

function ca_con_finish()
{
		document.forms[0].ca_finish.value=1;
		document.forms[0].dns_hijack.value=1;
		document.forms[0].submit_flag.value="ca_finish_url";
		if(mobile_tag == 1)
			document.forms[0].action="/apply.cgi?/mobile_ca_connect_uncomplete.htm timestamp="+ts;
		else
			document.forms[0].action="/apply.cgi?/ca_connect_uncomplete.htm timestamp="+ts;
		document.forms[0].submit();	
}

function ca_con_finish_wpa()
{
		document.forms[0].ca_finish.value=1;
		document.forms[0].dns_hijack.value=1;
		document.forms[0].submit_flag.value="ca_finish_url";
		if(mobile_tag == 1)
			document.forms[0].action="/apply.cgi?/mobile_ca_connect_uncomplete_wpa.htm timestamp="+ts;
		else
			document.forms[0].action="/apply.cgi?/ca_connect_uncomplete_wpa.htm timestamp="+ts;
		document.forms[0].submit();	
}

function msg_close()
{
	if( get_browser() == "IE" || get_browser() == "Firefox")
	{
		if (confirm("$ca_add_bookmark"))	
			addfavorite();
	}
//        alert(ca_close_alert);
		if( get_browser() == "Firefox" || mobile_tag == 1)
			alert("$ca_finish");
		else
        CloseWindow();
}
function CloseWindow()
{
    if(get_browser() != "Firefox"){
    	if (confirm("$ca_close_window")){
       	top.window.opener = null;
        top.window.open('','_self',''); 
        top.window.close();
    	}
    }
}

function addfavorite()
{
   if (document.all)
   {
      window.external.addFavorite('http://www.mywifiext.net','WN1000RP Configuration');
   }
   else if (window.sidebar)
   {
      window.sidebar.addPanel('WN1000RP Configuration', 'http://www.mywifiext.net', "");
   }	
}
function getObj(name)
{
    if (document.getElementById)
    {
        return document.getElementById(name).style;
    }
    else if (document.all)
    {
        return document.all[name].style;
    }
    else if (document.layers)
    {
        return document.layers[name];
    }
}
function check_maxNumAdd(form,nowNum,maxNum,msg,go2href)
{
	nowNum=parseInt(nowNum,10);
	if ( nowNum >= maxNum )
	{
		alert(msg);
		return false;
	}
	else
		location.href=go2href;
}
function checkIPMain(ip,max) {
    if( isNumeric(ip, max) == false ) 
        return false;
}
 function country_name(country_code)
{
	   if(country_code==0)
	   document.write(coun_af);
	   else if(country_code==1)
	   document.write(coun_as);
	   else if(country_code==2)
	   document.write(coun_au);
	   else if(country_code==3)
	   document.write(coun_ca);
	   else if(country_code==4)
	   document.write(coun_eu);
	   else if(country_code==5)
	   document.write(coun_is);
	   else if(country_code==6)
	   document.write(coun_jp);
	   else if(country_code==7)
	   document.write(coun_ko);
	   else if(country_code==8)
	   document.write(coun_mx);
	   else if(country_code==9)
	   document.write(coun_sa);
	   else if(country_code==10)
	   document.write(coun_us);
	   else if(country_code==11)
	   document.write(coun_me);
	   else if(country_code==13)
	   {
	   if(GUI_Region=="English")
	   document.write(coun_eu);
	   else
	   document.write(coun_fr);
	   }
	   else
	   document.write(coun_eu);
}

	function checkIP(ip1, ip2, ip3, ip4,max) {
    if(checkIPMain(ip1,255)==false) return false; 
    if(checkIPMain(ip2,255)==false) return false;
    if(checkIPMain(ip3,255)==false) return false;
    if(checkIPMain(ip4,max)==false) return false;
    if((parseInt(ip1)==0)||(parseInt(ip1)==0)&&(parseInt(ip2)==0)&&(parseInt(ip3)==0)&&(parseInt(ip4)==0))
    	return false;
	if ( ip1 == 0 )
		return false;
    return true;
}

/* Check Numeric*/
function isNumeric(str, max) 
{
    if(str.length == 0 || str == null || str == "") 
        return false;
    for(i=0; i<str.length; i++) 
	{
        var c = str.substring(i, i+1);
        if("0" <= c && c <= "9") 
           	continue;
		else
        	return false;
    	}
    var i = parseInt(str);
    if(i>max) 
        return false;
   
    return true;
}

function isIE()
{
	var browser = new Object();
	browser.version = parseInt(navigator.appVersion);
	browser.isNs = false;
	browser.isIe = false;
	if(navigator.appName.indexOf("Netscape") != -1)
		browser.isNs = true;
	else if(navigator.appName.indexOf("Microsoft") != -1)
		browser.isIe = true;
			
	if(browser.isNs)
		return false;
	else if (browser.isIe)
		return true;
	else
		return false;
}
function telnet_apply(cf)
{
if (cf.enable_telnet.checked==true)
cf.enable_telnet.value=1;
else
cf.enable_telnet.value=0;
cf.submit();
}

function icons_show_status()
{
	//document.write("<IMG height=40 src=/router.jpg width=80% border=0><br>");
	if (link_status==0)
		document.write("<IMG height=40 src=/router_link_off.jpg width=80% border=0><br>");
	else if (link_status==1)
		document.write("<IMG height=40 src=/router_link_red.jpg width=80% border=0><br>");
	else if (link_status==2)
		document.write("<IMG height=40 src=/router_link_amber.jpg width=80% border=0><br>");
	else if (link_status==3)
		document.write("<IMG height=40 src=/router_link_green.jpg width=80% border=0><br>");
	document.write("<IMG height=40 src=/repeater_green.jpg width=80% border=0><br>");
	if (sta_list==1)
		document.write("<IMG height=40 src=/client_link_green.jpg width=80% border=0><br>");
	else
		document.write("<IMG height=40 src=/client_link_off.jpg width=80% border=0><br>");
	//document.write("<IMG height=40 src=/client.jpg width=80% border=0><br>");
}

function load_default(step){
	var menu_div;
	var content_length;
	if (top.document.getElementsByName("IA_menu_1")){
		for(var i=1;i<=3;i++){
			eval('top.document.getElementsByName("IA_menu_'+i+'")[0]').className ="wizard_gray_button";
		}
		if(gui_region == "Russian" || gui_region == "Japanese" || ((gui_region == "Korean" || gui_region == "Spanish" || gui_region == "Italian") && IE_version() == 9))
			eval('top.document.getElementsByName("IA_menu_1")[0]').className ="wizard_gray_long_button";
		else
			eval('top.document.getElementsByName("IA_menu_1")[0]').className ="wizard_gray_button";
		if(gui_region == "Italian" || gui_region == "French" || gui_region == "Russian" || gui_region == "Spanish" || gui_region == "Japanese")
			eval('top.document.getElementsByName("IA_menu_2")[0]').className ="wizard_gray_long_button";
		else
			eval('top.document.getElementsByName("IA_menu_2")[0]').className ="wizard_gray_button";
		if(gui_region == "French" || gui_region == "German" || ((gui_region == "Russian" || gui_region == "Spanish" || gui_region == "Italian") && IE_version() == 9))
			eval('top.document.getElementsByName("IA_menu_3")[0]').className ="wizard_gray_long_button";
		else
			eval('top.document.getElementsByName("IA_menu_3")[0]').className ="wizard_gray_button";
	
		eval('top.document.getElementsByName("IA_menu_'+step+'")[0]').className ="wizard_purple_button";
		if( (gui_region == "German" && step == 3) || (gui_region == "Russian" && (step == 1 || step == 2)) || (gui_region == "Spanish" && step == 2) || (gui_region == "Japanese" && (step == 1 || step == 2)) || (gui_region == "French" && (step == 2 || step == 3)) || (gui_region == "Italian" && step == 2) || (gui_region == "Korean" && IE_version() == 9 && step == 1) || (gui_region == "Russian" && IE_version() == 9 && step == 3) || ((gui_region == "Spanish" || gui_region == "Italian") && IE_version() == 9 && (step == 1 || step == 3)))
			eval('top.document.getElementsByName("IA_menu_'+step+'")[0]').className ="wizard_purple_long_button";
	}
}

function _isNumeric(str) {
    var i;
    for(i = 0; i<str.length; i++) {
        var c = str.substring(i, i+1);
        if("0" <= c && c <= "9") {
            continue;
        }
        return false;
    }
    return true;
}

function isSameIp(ipstr1,ipstr2)
{
        var count = 0;
        var ip1_array=ipstr1.split('.');
        var ip2_array=ipstr2.split('.');
        for(i = 0;i<4;i++)
        {
                num1 = parseInt(ip1_array[i]);
                num2 = parseInt(ip2_array[i]);
                if( num1 == num2)
                        count++;
        }
        if( count == 4)
                return true;
        else
                return false;
}

function cp_ip2(from,to)
//true invalid from and to ip;  false valid from and to ip;
{
    var total1 = 0;
    var total2 = 0;
	var from_array=from.split('.');
	var to_array = to.split('.');
	var from1=from_array[0];
	var from2=from_array[1];
	var from3=from_array[2];
	var from4=from_array[3];
	var to1=to_array[0];
	var to2=to_array[1];
	var to3=to_array[2];
	var to4=to_array[3];

    total1 += parseInt(from4);
    total1 += parseInt(from3)*256;
    total1 += parseInt(from2)*256*256;
    total1 += parseInt(from1)*256*256*256;

    total2 += parseInt(to4);
    total2 += parseInt(to3)*256;
    total2 += parseInt(to2)*256*256;
    total2 += parseInt(to1)*256*256*256;
    if(total1 <= total2)
        return true;
    return false;
}


function isSameSubNet(lan1Ip, lan1Mask, lan2Ip, lan2Mask)
{       
        var count = 0;
		var count_error_end = 0;
		var count_error_start = 0;
        lan1a = lan1Ip.split('.');
        lan1m = lan1Mask.split('.');
        lan2a = lan2Ip.split('.');
        lan2m = lan2Mask.split('.');
        
        for (i = 0; i < 4; i++)
        {       
                l1a_n = parseInt(lan1a[i]);
                l1m_n = parseInt(lan1m[i]);
                l2a_n = parseInt(lan2a[i]);
                l2m_n = parseInt(lan2m[i]);
                if ((l1a_n & l1m_n) == (l2a_n & l2m_n))
                        count++;
				lan_error_start=(l1a_n & l1m_n);
				var l2a_n_two=0;
				var l2m_n_two=0;
				
				
				var rev = ~l2m_n;
				rev=rev+256;
               
				
				lan_error_end=(rev|l2a_n);
				if (lan_error_end==l2a_n)
				    count_error_end++;
				if (lan_error_start==l2a_n)
				    count_error_start++;
				if (count_error_end == 4)
				   return false;
				if (count_error_start == 4)
				   return false;
	            
        }
        if (count == 4)
               return true;
	    
		
        else
               return false;
}

function wlan_txctrl(form, tx_power_ctrl, tx_power_ctrl_an, wla_channel, country)
{
        if((netgear_region == "WW" || netgear_region == "") && (country != "3" && country != "10"))
        {
		if(tx_power_ctrl == "100")
                        form.wl_tx_ctrl.value="44";
                else if(tx_power_ctrl == "75")
                        form.wl_tx_ctrl.value="10.5";
                else if(tx_power_ctrl == "50")
                        form.wl_tx_ctrl.value="7";
                else if(tx_power_ctrl == "25")
                        form.wl_tx_ctrl.value="3.5";

                //for 5G
                if(tx_power_ctrl_an == "100")
                        form.wla_tx_ctrl.value="44";
                if(parseInt(wla_channel) >= 36 && parseInt(wla_channel) <= 64)
                {
                        if(tx_power_ctrl_an == "75")
                                form.wla_tx_ctrl.value="12.5";
                        else if(tx_power_ctrl_an == "50")
                                form.wla_tx_ctrl.value="8.5";
                        else if(tx_power_ctrl_an == "25")
                                form.wla_tx_ctrl.value="4";
                }
                else if(parseInt(wla_channel) >= 100)
                {
                        if(tx_power_ctrl_an == "75")
                                form.wla_tx_ctrl.value="15.5";
                        else if(tx_power_ctrl_an == "50")
                                form.wla_tx_ctrl.value="10.5";
                        else if(tx_power_ctrl_an == "25")
                                form.wla_tx_ctrl.value="5";
		}
	}
        else
        {
                if(tx_power_ctrl == "100")
                        form.wl_tx_ctrl.value="44";
                else if(tx_power_ctrl == "75")
                        form.wl_tx_ctrl.value="18";
                else if(tx_power_ctrl == "50")
                        form.wl_tx_ctrl.value="12";
                else if(tx_power_ctrl == "25")
                        form.wl_tx_ctrl.value="6";

		//for 5G
                if(tx_power_ctrl_an == "100")
                        form.wla_tx_ctrl.value="44";
                if(parseInt(wla_channel) >= 36 && parseInt(wla_channel) <= 48)
                {
                        if(tx_power_ctrl_an == "75")
                                form.wla_tx_ctrl.value="10.5";
                        else if(tx_power_ctrl_an == "50")
                                form.wla_tx_ctrl.value="7";
                        else if(tx_power_ctrl_an == "25")
                                form.wla_tx_ctrl.value="3.5";
                }
                else if(parseInt(wla_channel) >= 52 && parseInt(wla_channel) <= 165)
                {
                        if(tx_power_ctrl_an == "75")
                                form.wla_tx_ctrl.value="15.5";
                        else if(tx_power_ctrl_an == "50")
                                form.wla_tx_ctrl.value="10.5";
                        else if(tx_power_ctrl_an == "25")
                                form.wla_tx_ctrl.value="5";
                }
	}
	
	//the current values should be changed to (urrent_value+6)*2.
	form.wl_tx_ctrl.value = ( parseFloat(form.wl_tx_ctrl.value) + 6 ) * 2;
	form.wla_tx_ctrl.value = ( parseFloat(form.wla_tx_ctrl.value) + 6 ) * 2;
	
}

function checkipaddr(ipaddr)
{
	var form = document.forms[0];
	var ipArray = ipaddr.split(".");
	var ipstr = ipArray[0]+ipArray[1]+ipArray[2]+ipArray[3];
	var i = 0;

	if((ipArray[0]=="")||(ipArray[0]<0)||(ipArray[0]>255)||(ipArray[1]=="")||(ipArray[1]<0)||(ipArray[1]>255)
 	||(ipArray[2]=="")||(ipArray[2]<0)||(ipArray[2]>255)||(ipArray[3]=="")||(ipArray[3]<0)||(ipArray[3]>255))
	{
		return false;
	}
	for(i=0;i<ipstr.length;i++)
      	{
        	if((ipstr.charAt(i)!='0')&&(ipstr.charAt(i)!='1')&&(ipstr.charAt(i)!='2')
         	&&(ipstr.charAt(i)!='3')&&(ipstr.charAt(i)!='4')&&(ipstr.charAt(i)!='5')
         	&&(ipstr.charAt(i)!='6')&&(ipstr.charAt(i)!='7')&&(ipstr.charAt(i)!='8')
         	&&(ipstr.charAt(i)!='9'))
         	{
            		return false;
         	}
      	}
	if( ipArray[0] > 223 || ipArray[0] == 0 )
		return false;
	if (ipaddr == "0.0.0.0" || ipaddr == "255.255.255.255")
	{
        	return false;
	}
	//bug 24608 static ip address can't set with loopback address
	var each=ipaddr.split(".");
	if (each[0] == "127")
	{
		return false;
	}
	if (!ipArray || ipArray.length != 4)
      	{
           	return false;
	}
	else
	{
		for (i = 0; i < 4; i++)
        	{
          		thisSegment = ipArray[i];
          		if (thisSegment != "")
          		{
             			if(i==3){
                			if (!((ipArray[3] > 0) && (ipArray[3] < 255)))
                			{
                  				return false;
                			}	
				}
		             	else if (!(thisSegment >=0 && thisSegment <= 255))
             			{
                			return false;
             			}
          		} 
			else
          		{
            			return false;
          		}
        	}

	}
	return true;
}

function port_range_interception(port)
{
	port=parseInt(port,10);
	return port;
}

function checksubnet(subnet)
{
        var subnetArray = subnet.split(".");
        var subnetstr = subnetArray[0]+subnetArray[1]+subnetArray[2]+subnetArray[3];
        var i = 0;
        var maskTest = 0;
        var validValue = true;

        if((subnetArray[0]=="")||(subnetArray[0]<0)||(subnetArray[0]>255)||(subnetArray[1]=="")||(subnetArray[1]<0)||(subnetArray[1]>255)
        ||(subnetArray[2]=="")||(subnetArray[2]<0)||(subnetArray[2]>255)||(subnetArray[3]=="")||(subnetArray[3]<0)||(subnetArray[3]>255))
        {
                return false;
        }
        for(i=0;i<subnetstr.length;i++)
        {
                if((subnetstr.charAt(i)!='0')&&(subnetstr.charAt(i)!='1')&&(subnetstr.charAt(i)!='2')
                &&(subnetstr.charAt(i)!='3')&&(subnetstr.charAt(i)!='4')&&(subnetstr.charAt(i)!='5')
                &&(subnetstr.charAt(i)!='6')&&(subnetstr.charAt(i)!='7')&&(subnetstr.charAt(i)!='8')
                &&(subnetstr.charAt(i)!='9'))
                {
                        return false;
                }
        }
	if (!subnetArray || subnetArray.length != 4)
        {
                return false;
        }
	else
        {
                for (i = 0; i < 4; i++) {
                        thisSegment = subnetArray[i];
                        if (thisSegment != "") {
                                if (!(thisSegment >=0 && thisSegment <= 255)) { //check if number?
                             
                                        return false;
                                }
                        } else {
                                return false;
                        }
		}
        }
        if( subnetArray[0] < 255 ) 
	{
                if( (subnetArray[1] > 0) || (subnetArray[2] > 0) || (subnetArray[3] > 0))
                        validValue = false;
                else
                        maskTest = subnetArray[0];
        } 
	else 
	{
                if( subnetArray[1] < 255 ) 
		{
                        if( (subnetArray[2] > 0) || (subnetArray[3] > 0))
                                validValue = false;
                        else
                                maskTest = subnetArray[1];
                } 
		else
		{
                        if( subnetArray[2] < 255 ) 
			{
                                if( (subnetArray[3] > 0) )
                                        validValue = false;
                                else
                                        maskTest = subnetArray[2];
                        } 
			else
                                maskTest = subnetArray[3];
                }
        }
        if( validValue ) {
                switch( maskTest ) {
                case "0":
                case "128":
                case "192":
                case "224":
                case "240":
                case "248":
                case "252":
                case "254":
                case "255":
                        break;
                default:
                        validValue = false;
                }
                if( subnet == "0.0.0.0" )
                        validValue = false;
        }
        else
                validValue = false;


        return validValue;
}

function checkgateway(gateway)
{
	var form = document.forms[0];
	var dgArray = gateway.split(".");
        var dgstr = dgArray[0]+dgArray[1]+dgArray[2]+dgArray[3];
	var i = 0;	

	if((dgArray[0]=="")||(dgArray[0]<0)||(dgArray[0]>255)||(dgArray[1]=="")||(dgArray[1]<0)||(dgArray[1]>255)
 	||(dgArray[2]=="")||(dgArray[2]<0)||(dgArray[2]>255)||(dgArray[3]=="")||(dgArray[3]<0)||(dgArray[3]>255))
	{
		return false;
	}	
	for(i=0;i<dgstr.length;i++)
      	{
        	if((dgstr.charAt(i)!='0')&&(dgstr.charAt(i)!='1')&&(dgstr.charAt(i)!='2')
         	&&(dgstr.charAt(i)!='3')&&(dgstr.charAt(i)!='4')&&(dgstr.charAt(i)!='5')
         	&&(dgstr.charAt(i)!='6')&&(dgstr.charAt(i)!='7')&&(dgstr.charAt(i)!='8')
         	&&(dgstr.charAt(i)!='9'))
         	{
            		return false;
         	}
      	}
	if( dgArray[0] > 223 || dgArray[0] == 0 )
                return false;
        if (gateway == "0.0.0.0" || gateway == "255.255.255.255")
        {
                return false;
        }
        if (gateway == "127.0.0.1")
        {
                return false;
        }
	if (!dgArray || dgArray.length != 4)
        {
                return false;
        }
        else
        {
                for (i = 0; i < 4; i++) {
                        thisSegment = dgArray[i];
                        if (thisSegment != "") {
                                if (!(thisSegment >=0 && thisSegment <= 255)) { //check if number?
                            
                                        return false;
                                }
                        } else {
                                return false;
                        }
				}
        }
	return true;
}
function isBroadcast(lanIp, lanMask)
{
        ip_arr = lanIp.split('.');
        mask_arr = lanMask.split('.');
        var ip_broadcast=0
        for (i = 0; i < 4; i++)
        {
                ip_str = parseInt(ip_arr[i]);
                mask_str = parseInt(mask_arr[i]);
				n_str = ~mask_str+256; 	
				ip_broadcast=ip_broadcast*256+parseInt(ip_str | n_str)              
        }
		return(ip_broadcast);
}

function isSub(lanIp, lanMask)
{
        ip_arr = lanIp.split('.');
        mask_arr = lanMask.split('.');
        var ip_sub=0
        for (i = 0; i < 4; i++)
        {
                ip_str = parseInt(ip_arr[i]);
                mask_str = parseInt(mask_arr[i]);
				ip_sub=ip_sub*256+parseInt(ip_str & mask_str)              
        }
		return(ip_sub)
}

function isGateway(lanIp, lanMask,gtwIp)
{
		gtw_arr = gtwIp.split('.');
		var ip_gtw=0
		for (i = 0; i < 4; i++)
        {
                gtw_str = parseInt(gtw_arr[i]);
				ip_gtw=ip_gtw*256+parseInt(gtw_str);              
        }
		var ip_sub=isSub(lanIp, lanMask);
		var ip_broadcast=isBroadcast(lanIp, lanMask)
		if((parseInt(ip_sub)<parseInt(ip_gtw))&&(parseInt(ip_gtw)<parseInt(ip_broadcast)))
			return true;
		else
			return false;
}
function loadhelp(fname,anchname)
{
	var page="/help/"+fname+"_help.html";
	
	if(this.help_iframe)
	{
		this.help_iframe.location.href=page+"#"+ anchname;
		this.help_iframe.location.href=page+"#"+ anchname; // to fix bug 29847.
	}
	return;
	
	/*parent.help_info = fname;
	if(parent.parent.help_info)
		parent.parent.help_info = fname;
        if ((loadhelp.arguments.length == 1 ) || (anchname == "" )){
	    	top.helpframe.location.href="/cgi-bin/help.htm"; 
		//"/h"+fname+".htm";
	}
        else{
                top.helpframe.location.href="/cgi-bin/help.htm#" + anchname;
		//"/h"+fname+".htm#" + anchname;
	}
   	return;*/
}

function getkeya(e)
{
var keycode;
  if (window.event) 
  {
    keycode = window.event.keyCode;
    if (((keycode>47) && (keycode<58))||(keycode==8)||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))) { return true; }
    else return false;
  }
  else if (e) 
  {
    keycode = e.which;
    if (((keycode>47) && (keycode<58))||(keycode==8)||(keycode==0)||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))) { return true; }
    else return false;
  }
  else 
  {
    return true;
  }
}

function isValidChar_keytype(each_char)
{
	if (((each_char>47) && (each_char<58))||((each_char>64) && (each_char<71))||((each_char>96) && (each_char<103))||(each_char == 8)||(each_char == 0)) 
		return true;
	else return false;
}

function getkey(type, e)
{
	var keycode;
	if (window.event) keycode = window.event.keyCode;
	else if (e) keycode = e.which;
	else return true;
	
	if(type == "apname")
	{
		if ((keycode==34) ||(keycode==39)||(keycode==92)) { return false; }
		else return true;
	}
	else if(type == "ipaddr")
	{	
		if (((keycode>47) && (keycode<58))||(keycode==8)||(keycode==0)||(keycode==46)) { return true; }
		else return false;
	}
	else if(type == "ssid")
	{
		if (keycode==32) return false;
		else return true;
	}
	else if(type == "wep1")
	{
		var len1=document.getElementById("key1_id").value.length;
		if(getObj("keylen_id").options[0].selected == true)
		{
			for(i=0;i<5;i++)
			{
				if(isValidChar_keytype(getObj("key1_id").value.charCodeAt(i)) == false)
				{
					document.getElementById("key1_id").maxLength=5;
					return true;
				}
			}
			document.getElementById("key1_id").maxLength=10;
			if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0)) 
				return true;
			else return false;
		}
		else
		{
			for(i=0;i<13;i++)
			{
				if(isValidChar_keytype(getObj("key1_id").value.charCodeAt(i)) == false)
				{
					document.getElementById("key1_id").maxLength=13;
					return true;
				}
			}
			document.getElementById("key1_id").maxLength=26;
			if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0)) 
				return true;
			else return false;			
		}
	}
	else if(type == "wep2")
	{
		var len2=document.getElementById("key2_id").value.length;
		if(getObj("keylen_id").options[0].selected == true)
		{
			for(i=0;i<5;i++)
			{
				if(isValidChar_keytype(getObj("key2_id").value.charCodeAt(i)) == false)
				{
					document.getElementById("key2_id").maxLength=5;
					return true;
				}
			}
			document.getElementById("key2_id").maxLength=10;
			if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0)) 
				return true;
			else return false;
		}
		else
		{
			for(i=0;i<13;i++)
			{
				if(isValidChar_keytype(getObj("key2_id").value.charCodeAt(i)) == false)
				{
					document.getElementById("key2_id").maxLength=13;
					return true;
				}
			}
			document.getElementById("key2_id").maxLength=26;
			if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0)) 
				return true;
			else return false;
		}
	}
	else if(type == "wep3")
	{
		var len3=document.getElementById("key3_id").value.length;
		if(getObj("keylen_id").options[0].selected == true)
		{
			for(i=0;i<5;i++)
			{
				if(isValidChar_keytype(getObj("key3_id").value.charCodeAt(i)) == false)
				{
					document.getElementById("key3_id").maxLength=5;
					return true;
				}
			}
			document.getElementById("key3_id").maxLength=10;
			if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0)) 
				return true;
			else return false;
		}
		else
		{
			for(i=0;i<13;i++)
			{
				if(isValidChar_keytype(getObj("key3_id").value.charCodeAt(i)) == false)
				{
					document.getElementById("key3_id").maxLength=13;
					return true;
				}
			}
			document.getElementById("key3_id").maxLength=26;
			if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0)) 
				return true;
			else return false;
		}
	}
	else if(type == "wep4")
	{
		var len4=document.getElementById("key4_id").value.length;
		if(getObj("keylen_id").options[0].selected == true)
		{
			for(i=0;i<5;i++)
			{
				if(isValidChar_keytype(getObj("key4_id").value.charCodeAt(i)) == false)
				{
					document.getElementById("key4_id").maxLength=5;
					return true;
				}
			}
			document.getElementById("key4_id").maxLength=10;
			if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0)) 
				return true;
			else return false;
		}
		else
		{
			for(i=0;i<13;i++)
			{
				if(isValidChar_keytype(getObj("key4_id").value.charCodeAt(i)) == false)
				{
					document.getElementById("key4_id").maxLength=13;
					return true;
				}
			}
			document.getElementById("key4_id").maxLength=26;
			if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0)) 
				return true;
			else return false;
		}
	}
	else if(type == "num")	
	{	
		if	(((keycode>47) && (keycode<58)) || (keycode==8)||(keycode==0))
			return true;		
		else return false;	
	}
	else if(type == "num_space")
	{
		if (((keycode>47) && (keycode<58)) || (keycode==8)||(keycode==0) || (keycode == 32) || (keycode == 45))
			return true;
		else return false;
	}
	else if(type == "hostname")
	{
		if (((keycode>47) && (keycode<58))||(keycode==45)||((keycode>64) && (keycode<91))||((keycode>96) && (keycode<123)) || (keycode==8)||(keycode==0)) { return true; }
		else return false;
	}
	else if(type == "ddns_hostname")
	{
		if (((keycode>47) && (keycode<58))||(keycode==45)||(keycode==46)||((keycode>64) && (keycode<91))||((keycode>96) && (keycode<123)) || (keycode==8)||(keycode==0)) { return true; }
		else return false;
	}
	else if(type == "mac")
	{
		if (((keycode>47) && (keycode<58))||((keycode>64) && (keycode<71))||((keycode>96) && (keycode<103))||(keycode == 8)||(keycode == 0) || (keycode == 58) || (keycode == 45))
                        return true;
                else return false;
	}
	else	
		return false;
}



function changesectype(fname)
{
	var html_href;
	if(fname.options[0].selected == true) html_href = "security_off.htm";
	else if(fname.options[1].selected == true) html_href = "wep.htm";
	else if(fname.options[2].selected == true) html_href = "wpa.htm";
	else if(fname.options[3].selected == true) html_href = "wpa2.htm";
	else if(fname.options[4].selected == true) html_href = "wpas.htm";
	
	location.href = html_href;
}

function printableKeyFilter() 
{
	if (event.keyCode < 32 || event.keyCode > 126)
    		event.returnValue = false;
}

function check_capsk(form)
{
    var len = form.sec_wpaphrase.value.length;
    if ( len == 64 )
    {
        for(i=0;i<len;i++)
        {
            if(isValidHex(form.sec_wpaphrase.value.charAt(i))==false)
            {
                alert(wpa_phrase);
                return false;
            }
        }
    }
    else
    {
        if(len < 8 || len > 63){
            alert(wpa_phrase);
            return false;
        }
        for(i=0;i<form.sec_wpaphrase.value.length;i++)
             {
                    if(isValidChar_space(form.sec_wpaphrase.value.charCodeAt(i))==false)
                    {
                            alert(wpa_phrase);
                            return false;
                    }
            }
    }
    form.sec_wpaphrase_len.value=len;
    return true;
}

function checkpsk(passphrase, passphrase_len)
{
	var len = passphrase.value.length;
	if ( len == 64 )
	{
		for(i=0;i<len;i++)
		{
			if(isValidHex(passphrase.value.charAt(i))==false)
			{
				alert("$notallowpassps");
				return false;
			}
		}
	}
	else
	{
		if(len < 8 )
		{
	        	alert("$passphrase_short8");
		        return false;
		} 
		if(len > 63){
			alert("$passphrase_long63");
			return false;
		}
		for(i=0;i<passphrase.value.length;i++)
		{
			if(isValidChar_space(passphrase.value.charCodeAt(i))==false)
			{
				alert("$notallowpassps");
				return false;
			}
		}
	}
	passphrase_len.value=len;
	return true;
}

/*function checkpsk(form)
{
    if(getObj("type_id3").checked == true)
	{
    var len = getObj("textWpaPwdPhrase1").value.length;
	phrase_value=getObj("textWpaPwdPhrase1").value;
	}
	else if(getObj("type_id4").checked == true)
	{
	var len = getObj("textWpaPwdPhrase2").value.length;
	phrase_value=getObj("textWpaPwdPhrase2").value;
	}
	else if(getObj("type_id6").checked == true)
	{
	var len = getObj("textWpaPwdPhrase3").value.length;
	phrase_value=getObj("textWpaPwdPhrase3").value;
    }
	else if(getObj("type_id7").checked == true)
	{
	var len = getObj("textWpaPwdPhrase4").value.length;
	phrase_value=getObj("textWpaPwdPhrase4").value;
    }
	if ( len == 64 )
    {
        for(i=0;i<len;i++)
        {
            if(isValidHex(phrase_value.charAt(i))==false)
            {
                alert("$wpa_phrase");
                return false;
            }
        }
    }
    else
    {
        if(len < 8 || len > 63){
            alert("$wpa_phrase");
            return false;
        }
        for(i=0;i<phrase_value.length;i++)
             {
                    if(isValidChar_space(phrase_value.charCodeAt(i))==false)
                    {
                            alert("$wpa_phrase");
                            return false;
                    }
            }
    }
    form.sec_wpaphrase_len.value=len;
    return true;
}*/

function checkpsk_ca_fixed(form)
{
	var len = getObj("textWpaPwdPhrase").value.length;
	phrase_value=getObj("textWpaPwdPhrase").value;
	if ( len == 64 )
    {
        for(i=0;i<len;i++)
        {
            if(isValidHex(phrase_value.charAt(i))==false)
            {
                alert("$wpa_phrase");
                return false;
            }
        }
    }
    else
    {
        if(len < 8 || len > 63){
            alert("$wpa_phrase");
            return false;
        }
        for(i=0;i<phrase_value.length;i++)
             {
                    if(isValidChar_space(phrase_value.charCodeAt(i))==false)
                    {
                            alert("$wpa_phrase");
                            return false;
                    }
            }
    }
    form.sec_wpaphrase_len.value=len;
    return true;
}


function checkpsk_ca_manual(form)
{
    if(getObj("type_id").selectedIndex == 3)
	{
    var len = getObj("textWpaPwdPhrase1").value.length;
	phrase_value=getObj("textWpaPwdPhrase1").value;
	}
	else if(getObj("type_id").selectedIndex == 4)
	{
	var len = getObj("textWpaPwdPhrase2").value.length;
	phrase_value=getObj("textWpaPwdPhrase2").value;
	}
	else if(getObj("type_id").selectedIndex == 5)
	{
	var len = getObj("textWpaPwdPhrase3").value.length;
	phrase_value=getObj("textWpaPwdPhrase3").value;
    }
	else if(getObj("type_id").selectedIndex == 6)
	{
	var len = getObj("textWpaPwdPhrase4").value.length;
	phrase_value=getObj("textWpaPwdPhrase4").value;
    }
	else if(getObj("type_id").selectedIndex == 7)
	{
	var len = getObj("textWpaPwdPhrase5").value.length;
	phrase_value=getObj("textWpaPwdPhrase5").value;
    }
	if ( len == 64 )
    {
        for(i=0;i<len;i++)
        {
            if(isValidHex(phrase_value.charAt(i))==false)
            {
                alert("$wpa_phrase");
                return false;
            }
        }
    }
    else
    {
        if(len < 8 || len > 63){
            alert("$wpa_phrase");
            return false;
        }
        for(i=0;i<phrase_value.length;i++)
             {
                    if(isValidChar_space(phrase_value.charCodeAt(i))==false)
                    {
                            alert("$wpa_phrase");
                            return false;
                    }
            }
    }
    form.sec_wpaphrase_len.value=len;
    return true;
}


function checkpsk_client_manual(form)
{
    var len = form.ap_sec_wpaphrase.value.length;
    if ( len == 64 )
    {
        for(i=0;i<len;i++)
        {
            if(isValidHex(form.ap_sec_wpaphrase.value.charAt(i))==false)
            {
                alert(wpa_phrase);
                return false;
            }
        }
    }
    else
    {
        if(len < 8 || len > 63){
            alert(wpa_phrase);
            return false;
        }
        for(i=0;i<form.ap_sec_wpaphrase.value.length;i++)
             {
                    if(isValidChar_space(form.ap_sec_wpaphrase.value.charCodeAt(i))==false)
                    {
                            alert(wpa_phrase);
                            return false;
                    }
            }
    }
    form.ap_sec_wpaphrase_len.value=len;
    return true;
}

var wep_128="$wep_128";
var wep_64="$wep_64";

function show_alert(wep_bit)
{
	if(wep_bit == 10)
		alert(wep_64);
	else if(wep_bit == 26)
		alert(wep_128);
}
function check_key(key,wep_bit)
{
	var ascii_len;
	var i;

	if(wep_bit == 10)
		ascii_len = 5;
	else if(wep_bit == 26)
		ascii_len = 13;

	if(key.length == wep_bit)
	{
		for(i=0;i<key.length;i++)
		{
			if(isValidHex(key.charAt(i))==false)
			{
				show_alert(wep_bit);
				return false;
			}
		}
	}
	else
	{
		if(key.length != 0 && key.length != wep_bit && key.length != ascii_len)
		{
			show_alert(wep_bit);
			return false;
		}

		for(i=0;i<key.length;i++)
		{
			if(isValidChar_space(key.charCodeAt(i))==false)
			{
				show_alert(wep_bit);
				return false;
			}
		}
	}
	return true;
}

function checkwep(form)
{
	form.wep_press_flag.value=0;
	var KEY1=form.KEY1.value;
	var KEY2=form.KEY2.value;
	var KEY3=form.KEY3.value;
	var KEY4=form.KEY4.value;
	if(form.wepenc.value==13)
	{
		if( form.wep_key_no[0].checked == true )
		{
			if(form.KEY1.value.length!=26 && form.KEY1.value.length!=13) 
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY1,26) == false)
			return false;		
		
		if( form.wep_key_no[1].checked == true )
		{
			if(form.KEY2.value.length!=26 && form.KEY2.value.length!=13)
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY2,26) == false)
			return false;
		
		if( form.wep_key_no[2].checked == true)
		{
			if(form.KEY3.value.length!=26 && form.KEY3.value.length!=13) 
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY3,26) == false)
			return false;
		
		if( form.wep_key_no[3].checked == true)
		{
			if(form.KEY4.value.length!=26 && form.KEY4.value.length!=13) 
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY4,26) == false)
			return false;
	}
	if(form.wepenc.value==5)
	{
		if( form.wep_key_no[0].checked == true)
		{
			if(form.KEY1.value.length!=10 && form.KEY1.value.length!=5) 
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY1,10) == false)
			return false;
		
		if( form.wep_key_no[1].checked == true)
		{
			if(form.KEY2.value.length!=10 && form.KEY2.value.length!=5) 
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY2,10) == false)
			return false;

		if( form.wep_key_no[2].checked == true)
		{
			if(form.KEY3.value.length!=10 && form.KEY3.value.length!=5) 
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY3,10) == false)
			return false;
		
		if( form.wep_key_no[3].checked == true)
		{
			if(form.KEY4.value.length!=10 && form.KEY4.value.length!=5) 
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY4,10) == false)
			return false;
	}	
	return true;
}

function checkwep_a(form)
{
	form.wep_press_flag.value=0;
	var KEY1=form.KEY1_an.value;
	var KEY2=form.KEY2_an.value;
	var KEY3=form.KEY3_an.value;
	var KEY4=form.KEY4_an.value;
	if(form.Wepenc_an.value==13)
	{
		if( form.wep_key_no_an[0].checked == true )
		{
			if(KEY1.length!=26 && KEY1.length!=13) 
			{
				alert(wep_128);
				return false;
			}
		}	
		if(check_key(KEY1,26) == false)
			return false;	

		if( form.wep_key_no_an[1].checked == true )
		{
			if(KEY2.length!=26 && KEY2.length!=13)
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY2,26) == false)
			return false;
		
		if( form.wep_key_no_an[2].checked == true)
		{
			if(KEY3.length!=26 && KEY3.length!=13) 
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY3,26) == false)
			return false;
	
		if( form.wep_key_no_an[3].checked == true)
		{
			if(KEY4.length!=26 && KEY4.length!=13) 
			{
				alert(wep_128);
				return false;
			}
		}
		if(check_key(KEY4,26) == false)
			return false;
	}
	if(form.Wepenc_an.value==5)
	{
		if( form.wep_key_no_an[0].checked == true)
		{
			if(KEY1.length!=10 && KEY1.length!=5) 
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY1,10) == false)
			return false;
		
		if( form.wep_key_no_an[1].checked == true)
		{
			if(KEY2.length!=10 && KEY2.length!=5) 
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY2,10) == false)
			return false;

		if( form.wep_key_no_an[2].checked == true)
		{
			if(KEY3.length!=10 && KEY3.length!=5) 
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY3,10) == false)
			return false;
		
		if( form.wep_key_no_an[3].checked == true)
		{
			if(KEY4.length!=10 && KEY4.length!=5) 
			{
				alert(wep_64);
				return false;
			}
		}
		if(check_key(KEY4,10) == false)
			return false;
	}	
	return true;
}

function checkwep_byid(form)
{
	form.wep_press_flag.value=0;
	var wepkey1=getObj('key1_id').value;
	var wepkey2=getObj('key2_id').value;
	var wepkey3=getObj('key3_id').value;
	var wepkey4=getObj('key4_id').value;
	var len_tag1=0;
	var len_tag2=0;
	var len_tag3=0;
	var len_tag4=0;
	var lentag1=0;
	var lentag2=0;
	var lentag3=0;
	var lentag4=0;
	if(getObj('keylen_id').value==13)
	{
		for(i=0;i<13;i++)
		{
			if(isValidChar_keytype(getObj("key1_id").value.charCodeAt(i)) == false)
			lentag1=1;
		}
		for(i=0;i<13;i++)
		{
			if(isValidChar_keytype(getObj("key2_id").value.charCodeAt(i)) == false)
			lentag2=1;
		}
		for(i=0;i<13;i++)
		{
			if(isValidChar_keytype(getObj("key3_id").value.charCodeAt(i)) == false)
			lentag3=1;
		}
		for(i=0;i<13;i++)
		{
			if(isValidChar_keytype(getObj("key4_id").value.charCodeAt(i)) == false)
			lentag4=1;
		}
		if( getObj('wepkeyno_id1').checked == true )
		{
			if((lentag1 == 0 && getObj('key1_id').value.length!=13 && getObj('key1_id').value.length!=26) || (lentag1 == 1 && getObj('key1_id').value.length!=13)) 
			{
				alert("$wep_128");
				return false;
			}
		}	
		else if( getObj('key1_id').value.length!=0 && ((lentag1 == 0 && getObj('key1_id').value.length!=13 && getObj('key1_id').value.length!=26) || (lentag1 == 1 && getObj('key1_id').value.length!=13)))
		{
			alert("$wep_128");
			return false;
		}
		if(getObj('key1_id').value.length!=0)
			for(i=13;i<wepkey1.length;i++)
			{
			    if(isValidHex(wepkey1.charAt(i))==false)
			    {
			        alert("$wep_128");
			        return false;
			    }
			}	
		
		if( getObj('wepkeyno_id2').checked == true )
		{
			if((lentag2 == 0 && getObj('key2_id').value.length!=13 && getObj('key2_id').value.length!=26) || (lentag2 == 1 && getObj('key2_id').value.length!=13))
			{
				alert("$wep_128");
				return false;
			}
		}
		else if( getObj('key2_id').value.length!=0 && ((lentag2 == 0 && getObj('key2_id').value.length!=13 && getObj('key2_id').value.length!=26) || (lentag2 == 1 && getObj('key2_id').value.length!=13))) 
		{
			alert("$wep_128");
			return false;
		}
		if(getObj('key2_id').value.length!=0)
			for(i=13;i<wepkey2.length;i++)
		    {
		        if(isValidHex(wepkey2.charAt(i))==false)
		        {
		            alert("$wep_128");
		            return false;
		        }
		    }
		if( getObj('wepkeyno_id3').checked == true)
		{
			if((lentag3 == 0 && getObj('key3_id').value.length!=13 && getObj('key3_id').value.length!=26) || (lentag3 == 1 && getObj('key3_id').value.length!=13)) 
			{
				alert("$wep_128");
				return false;
			}
		}
		else if( getObj('key3_id').value.length!=0 && ((lentag3 == 0 && getObj('key3_id').value.length!=13 && getObj('key3_id').value.length!=26) || (lentag3 == 1 && getObj('key3_id').value.length!=13))) 
		{
			alert("$wep_128");
			return false;
		}
		if(getObj('key3_id').value.length!=0)
			for(i=13;i<wepkey3.length;i++)
		    {
		        if(isValidHex(wepkey3.charAt(i))==false)
		        {
		            alert("$wep_128");
		            return false;
		        }
		    }
		if( getObj('wepkeyno_id4').checked == true)
		{
			if((lentag4 == 0 && getObj('key4_id').value.length!=13 && getObj('key4_id').value.length!=26) || (lentag4 == 1 && getObj('key4_id').value.length!=13)) 
			{
				alert("$wep_128");
				return false;
			}
		}
		else if( getObj('key4_id').value.length!=0 && ((lentag4 == 0 && getObj('key4_id').value.length!=13 && getObj('key4_id').value.length!=26) || (lentag4 == 1 && getObj('key4_id').value.length!=13))) 
		{
			alert("$wep_128");
			return false;
		}
		if(getObj('key4_id').value.length!=0)
			for(i=13;i<wepkey4.length;i++)
	        {
	            if(isValidHex(wepkey4.charAt(i))==false)
	            {
	                alert("$wep_128");
	                return false;
	            }
	        }
	}
	if(getObj('keylen_id').value==5)
	{
		for(i=0;i<5;i++)
		{
			if(isValidChar_keytype(getObj("key1_id").value.charCodeAt(i)) == false)
			len_tag1=1;
		}
		for(i=0;i<5;i++)
		{
			if(isValidChar_keytype(getObj("key2_id").value.charCodeAt(i)) == false)
			len_tag2=1;
		}
		for(i=0;i<5;i++)
		{
			if(isValidChar_keytype(getObj("key3_id").value.charCodeAt(i)) == false)
			len_tag3=1;
		}
		for(i=0;i<5;i++)
		{
			if(isValidChar_keytype(getObj("key4_id").value.charCodeAt(i)) == false)
			len_tag4=1;
		}
		if( getObj('wepkeyno_id1').checked == true)
		{
			if((len_tag1 == 0 && getObj('key1_id').value.length!=5 && getObj('key1_id').value.length!=10) || (len_tag1 == 1 && getObj('key1_id').value.length!=5)) 
			{
				alert("$wep_64");
				return false;
			}
		}
		else if( getObj('key1_id').value.length!=0 && ((len_tag1 == 0 && getObj('key1_id').value.length!=5 && getObj('key1_id').value.length!=10) || (len_tag1 == 1 && getObj('key1_id').value.length!=5))) 
		{
			alert("$wep_64");
			return false;
		}
		if(getObj('key1_id').value.length!=0)
			for(i=5;i<wepkey1.length;i++)
		    {
		        if(isValidHex(wepkey1.charAt(i))==false)
	            {
	                alert("$wep_64");
	                return false;
	            }
	        }
		if( getObj('wepkeyno_id2').checked == true)
		{
			if((len_tag2 == 0 && getObj('key2_id').value.length!=5 && getObj('key2_id').value.length!=10) || (len_tag2 == 1 && getObj('key2_id').value.length!=5)) 
			{
				alert("$wep_64");
				return false;
			}
		}
		else if( getObj('key2_id').value.length!=0 && ((len_tag2 == 0 && getObj('key2_id').value.length!=5 && getObj('key2_id').value.length!=10) || (len_tag2 == 1 && getObj('key2_id').value.length!=5))) 
		{
			alert("$wep_64");
			return false;
		}
		if(getObj('key2_id').value.length!=0)
			for(i=5;i<wepkey2.length;i++)
	        {
	            if(isValidHex(wepkey2.charAt(i))==false)
	            {
	                alert("$wep_64");
	                return false;
				}
	        }

		if( getObj('wepkeyno_id3').checked == true)
		{
			if((len_tag3 == 0 && getObj('key3_id').value.length!=5 && getObj('key3_id').value.length!=10) || (len_tag3 == 1 && getObj('key3_id').value.length!=5)) 
			{
				alert("$wep_64");
				return false;
			}
		}
		else if( getObj('key3_id').value.length!=0 && ((len_tag3 == 0 && getObj('key3_id').value.length!=5 && getObj('key3_id').value.length!=10) || (len_tag3 == 1 && getObj('key3_id').value.length!=5))) 
		{
			alert("$wep_64");
			return false;
		}
		if(getObj('key3_id').value.length!=0)
			for(i=5;i<wepkey3.length;i++)
	        {
	            if(isValidHex(wepkey3.charAt(i))==false)
	            {
	                alert("$wep_64");
	                return false;
	            }
	        }
		if( getObj('wepkeyno_id4').checked == true)
		{
			if((len_tag4 == 0 && getObj('key4_id').value.length!=5 && getObj('key4_id').value.length!=10) || (len_tag4 == 1 && getObj('key4_id').value.length!=5)) 
			{
				alert("$wep_64");
				return false;
			}
		}
		else if( getObj('key4_id').value.length!=0 && ((len_tag4 == 0 && getObj('key4_id').value.length!=5 && getObj('key4_id').value.length!=10) || (len_tag4 == 1 && getObj('key4_id').value.length!=5))) 
		{
			alert("$wep_64");
			return false;
		}
		if(getObj('key4_id').value.length!=0)
			for(i=5;i<wepkey4.length;i++)
   	        {
	            if(isValidHex(wepkey4.charAt(i))==false)
	            {
	                alert("$wep_64");
	                return false;
	            }
	        }
	}	
	/*if ( form.sec_auth.value == 1 )
	{
		if (!confirm("$wep_or_wps"))
			return false;
	}*/	
	return true;
}


/////////////////////////////////////generate wep key by md5////////////////////////////////////////////////////
function HexToAscii(F,I,S,D) {
        var temp1="";

        S = S.toUpperCase();

    var optionindex=getObj("keylen_id").selectedIndex;

    if( getObj("keylen_id").options[optionindex].value=="13" )
    {
        wordCount = 26;
    }
    else {
        wordCount = 10;
    }

        //if(F.keyno_11g[I].checked)
        if(1)
        {
                if( (S.length!=wordCount) )
                {
                        if(getObj("wepkeyno_id1").checked==true || getObj("wepkeyno_id2").checked==true || getObj("wepkeyno_id3").checked==true || getObj("wepkeyno_id4").checked==true)
                        {
                                var s="Hex type key length must be ";
                                alert(s + wordCount);
                        }
                        D.value="";
                        S="";
                        return S;
                }
                for(i=0;i<wordCount;i+=2)
                {
                        var c=S.charCodeAt(i);
                        var d=S.charCodeAt(i+1);

                        if( (c>=48)&&(c<=57) )
                                c=c-48;
                        else if( (c>=65)&&(c<=70) )
                                c=c-55;
                        else
                        {
                                var s="Over Hex range (0~F)";
                                alert(s);
                                return S;
                        }

                        if( (d>=48)&&(d<=57) )
                                d=d-48;
                        else if( (d>=65)&&(d<=70) )
                                d=d-55;
                        else
                        {
                                var s="Over Hex range (0~F)";
                                alert(s);
                                return S;
                        }
                        var value=c*16+d;

                        if( ((value>=0)&&(value<32)) || ((value>128)&&(value<=255)) )
                        {
                                temp1+=String.fromCharCode(92);
                                temp1+=S.substring(i,i+2);

                        }
                        else
                        {
                                if(value==92)
                                {
                                        temp1+=String.fromCharCode(value);
                                        temp1+=String.fromCharCode(value);
                                }
                                else
                                        temp1+=String.fromCharCode(value);
                        }
                }
                D.value=temp1;
        }
        return S;
}

function HexToAscii_ap(F,I,S,D) {
        var temp1="";

        S = S.toUpperCase();

    var optionindex=F.ap_sec_keylen.selectedIndex;

    if( F.ap_sec_keylen.options[optionindex].value=="13" )
    {
        wordCount = 26;
    }
    else {
        wordCount = 10;
    }

        //if(F.keyno_11g[I].checked)
        if(1)
        {
                if( (S.length!=wordCount) )
                {
                        if(F.ap_wepkeyno[I].checked)
                        {
                                var s="Hex type key length must be ";
                                alert(s + wordCount);
                        }
                        D.value="";
                        S="";
                        return S;
                }
                for(i=0;i<wordCount;i+=2)
                {
                        var c=S.charCodeAt(i);
                        var d=S.charCodeAt(i+1);

                        if( (c>=48)&&(c<=57) )
                                c=c-48;
                        else if( (c>=65)&&(c<=70) )
                                c=c-55;
                        else
                        {
                                var s="Over Hex range (0~F)";
                                alert(s);
                                return S;
                        }

                        if( (d>=48)&&(d<=57) )
                                d=d-48;
                        else if( (d>=65)&&(d<=70) )
                                d=d-55;
                        else
                        {
                                var s="Over Hex range (0~F)";
                                alert(s);
                                return S;
                        }
                        var value=c*16+d;

                        if( ((value>=0)&&(value<32)) || ((value>128)&&(value<=255)) )
                        {
                                temp1+=String.fromCharCode(92);
                                temp1+=S.substring(i,i+2);

                        }
                        else
                        {
                                if(value==92)
                                {
                                        temp1+=String.fromCharCode(value);
                                        temp1+=String.fromCharCode(value);
                                }
                                else
                                        temp1+=String.fromCharCode(value);
                        }
                }
                D.value=temp1;
        }
        return S;
}
/*function PassPhrase40(F)
{
        var seed = 0;
        var pseed = new Array(0, 0, 0, 0);
        var pkey = new Array(4);
        var asciiObj = new Array(4);
        //Length = getObj("weppassphrase_id").value.length;
				Length = document.getElementsByName("weppassphrase")[0].value.length;
				
        if(Length != 0) {
                for (i=0; i<Length; i++ ) {
                       // pseed[i%4] ^= getObj("weppassphrase_id").value.charCodeAt(i);
                        pseed[i%4] ^= document.getElementsByName("weppassphrase")[0].value.charCodeAt(i);
                }
                seed = pseed[0];
                seed += pseed[1] << 8;
                seed += pseed[2] << 16;
                seed += pseed[3] << 24;
        }

        //getObj("key1_id").value = getObj("key2_id").value = "";
        //getObj("key3_id").value = getObj("key4_id").value = "";
        
        document.getElementsByName("wepkey1")[0].value = document.getElementsByName("wepkey2")[0].value = "";
        document.getElementsByName("wepkey3")[0].value = document.getElementsByName("wepkey4")[0].value = "";

        // init key array
       // pkey[0] = getObj("key1_id");
       // pkey[1] = getObj("key2_id");
       // pkey[2] = getObj("key3_id");
       // pkey[3] = getObj("key4_id");
        
        pkey[0] = document.getElementsByName("wepkey1")[0];
        pkey[1] = document.getElementsByName("wepkey2")[0];
      	pkey[2] = document.getElementsByName("wepkey3")[0];
        pkey[3] = document.getElementsByName("wepkey4")[0];

        for(j=0; j<4; j++) {
                for (i=0; i<5 ;i++ )  {
                        seed = (214013 * seed) & 0xffffffff;

                        if(seed & 0x80000000) {
                                seed = (seed & 0x7fffffff) + 0x80000000 + 0x269ec3;
                        }
                        else {
                                seed = (seed & 0x7fffffff) + 0x269ec3;
                        }

                        temp = ((seed >> 16) & 0xff);
                        if(temp < 0x10) {
                                pkey[j].value += "0" + temp.toString(16).toUpperCase();
                        }
                        else {
                                pkey[j].value += temp.toString(16).toUpperCase();
                        }
                }
        }

        asciiObj[0] = "";
        asciiObj[1] = "";
        asciiObj[2] = "";
        asciiObj[3] = "";

        for(k=0; k<4; k++) {
                HexToAscii(F, k, pkey[k].value, asciiObj[k]);
        }

        wepkey1 = pkey[0].value;
        wepkey2 = pkey[1].value;
        wepkey3 = pkey[2].value;
        wepkey4 = pkey[3].value;
}*/

function PassPhrase40(passphraseStr, wepenc, wep_key_no, KEY1, KEY2, KEY3, KEY4)
{
	var seed = 0;
	var pseed = new Array(0, 0, 0, 0);
	var pkey = new Array(4);
	var asciiObj = new Array(4);
	Length = passphraseStr.value.length;

	if(Length != 0) {
		for (i=0; i<Length; i++ ) {
			pseed[i%4] ^= passphraseStr.value.charCodeAt(i);
		}
		seed = pseed[0];
		seed += pseed[1] << 8;
		seed += pseed[2] << 16;
		seed += pseed[3] << 24;
	}

	KEY1.value = KEY2.value = "";
	KEY3.value = KEY4.value = "";

	// init key array
	pkey[0] = KEY1;
	pkey[1] = KEY2;
	pkey[2] = KEY3;
	pkey[3] = KEY4;

	for(j=0; j<4; j++) {
		for (i=0; i<5 ;i++ )  {
			seed = (214013 * seed) & 0xffffffff;

			if(seed & 0x80000000) {
				seed = (seed & 0x7fffffff) + 0x80000000 + 0x269ec3;
			}
			else {
				seed = (seed & 0x7fffffff) + 0x269ec3;
			}

			temp = ((seed >> 16) & 0xff);
			if(temp < 0x10) {
				pkey[j].value += "0" + temp.toString(16).toUpperCase();
			}
			else {
				pkey[j].value += temp.toString(16).toUpperCase();
			}
		}
	}

	asciiObj[0] = "";
	asciiObj[1] = "";
	asciiObj[2] = "";
	asciiObj[3] = "";

	for(k=0; k<4; k++) {
		HexToAscii(wepenc, wep_key_no, k, pkey[k].value, asciiObj[k]);
	}

	wepkey1 = pkey[0].value;
	wepkey2 = pkey[1].value;
	wepkey3 = pkey[2].value;
	wepkey4 = pkey[3].value;
}

function PassPhrase40_ap(F)
{
        var seed = 0;
        var pseed = new Array(0, 0, 0, 0);
        var pkey = new Array(4);
        var asciiObj = new Array(4);
        Length = F.ap_sec_phrase.value.length;

        if(Length != 0) {
                for (i=0; i<Length; i++ ) {
                        pseed[i%4] ^= F.ap_sec_phrase.value.charCodeAt(i);
                }
                seed = pseed[0];
                seed += pseed[1] << 8;
                seed += pseed[2] << 16;
                seed += pseed[3] << 24;
        }

        F.ap_wepkey1.value = F.ap_wepkey2.value = "";
        F.ap_wepkey3.value = F.ap_wepkey4.value = "";

        // init key array
        pkey[0] = F.ap_wepkey1;
        pkey[1] = F.ap_wepkey2;
        pkey[2] = F.ap_wepkey3;
        pkey[3] = F.ap_wepkey4;

        for(j=0; j<4; j++) {
                for (i=0; i<5 ;i++ )  {
                        seed = (214013 * seed) & 0xffffffff;

                        if(seed & 0x80000000) {
                                seed = (seed & 0x7fffffff) + 0x80000000 + 0x269ec3;
                        }
                        else {
                                seed = (seed & 0x7fffffff) + 0x269ec3;
                        }

                        temp = ((seed >> 16) & 0xff);
                        if(temp < 0x10) {
                                pkey[j].value += "0" + temp.toString(16).toUpperCase();
                        }
                        else {
                                pkey[j].value += temp.toString(16).toUpperCase();
                        }
                }
        }

        asciiObj[0] = "";
        asciiObj[1] = "";
        asciiObj[2] = "";
        asciiObj[3] = "";

        for(k=0; k<4; k++) {
                HexToAscii_ap(F, k, pkey[k].value, asciiObj[k]);
        }

        ap_wepkey1 = pkey[0].value;
        ap_wepkey2 = pkey[1].value;
        ap_wepkey3 = pkey[2].value;
        ap_wepkey4 = pkey[3].value;
}
/*
 * A JavaScript implementation of the RSA Data Security, Inc. MD5 Message
 * Digest Algorithm, as defined in RFC 1321.
 * Copyright (C) Paul Johnston 1999 - 2000.
 * Updated by Greg Holt 2000 - 2001.
 * See http://pajhome.org.uk/site/legal.htm for details.
 */

/*
 * Convert a 32-bit number to a hex string with ls-byte first
 */
var hex_chr = "0123456789abcdef";
function rhex(num)
{
  str = "";
  for(j = 0; j <= 3; j++)
    str += hex_chr.charAt((num >> (j * 8 + 4)) & 0x0F) +
           hex_chr.charAt((num >> (j * 8)) & 0x0F);
  return str;
}

/*
 * Convert a string to a sequence of 16-word blocks, stored as an array.
 * Append padding bits and the length, as described in the MD5 standard.
 */
function str2blks_MD5(str)
{
  nblk = ((str.length + 8) >> 6) + 1;
  blks = new Array(nblk * 16);
  for(i = 0; i < nblk * 16; i++) blks[i] = 0;
  for(i = 0; i < str.length; i++)
    blks[i >> 2] |= str.charCodeAt(i) << ((i % 4) * 8);
  blks[i >> 2] |= 0x80 << ((i % 4) * 8);
  blks[nblk * 16 - 2] = str.length * 8;
  return blks;
}

/*
 * Add integers, wrapping at 2^32. This uses 16-bit operations internally 
 * to work around bugs in some JS interpreters.
 */
function add(x, y)
{
  var lsw = (x & 0xFFFF) + (y & 0xFFFF);
  var msw = (x >> 16) + (y >> 16) + (lsw >> 16);
  return (msw << 16) | (lsw & 0xFFFF);
}

/*
 * Bitwise rotate a 32-bit number to the left
 */
function rol(num, cnt)
{
  return (num << cnt) | (num >>> (32 - cnt));
}

/*
 * These functions implement the basic operation for each round of the
 * algorithm.
 */
function cmn(q, a, b, x, s, t)
{
  return add(rol(add(add(a, q), add(x, t)), s), b);
}
function ff(a, b, c, d, x, s, t)
{
  return cmn((b & c) | ((~b) & d), a, b, x, s, t);
}
function gg(a, b, c, d, x, s, t)
{
  return cmn((b & d) | (c & (~d)), a, b, x, s, t);
}
function hh(a, b, c, d, x, s, t)
{
  return cmn(b ^ c ^ d, a, b, x, s, t);
}
function ii(a, b, c, d, x, s, t)
{
  return cmn(c ^ (b | (~d)), a, b, x, s, t);
}

/*
 * Take a string and return the hex representation of its MD5.
 */
function calcMD5(str)
{
  x = str2blks_MD5(str);
  a =  1732584193;
  b = -271733879;
  c = -1732584194;
  d =  271733878;

  for(i = 0; i < x.length; i += 16)
  {
    olda = a;
    oldb = b;
    oldc = c;
    oldd = d;

    a = ff(a, b, c, d, x[i+ 0], 7 , -680876936);
    d = ff(d, a, b, c, x[i+ 1], 12, -389564586);
    c = ff(c, d, a, b, x[i+ 2], 17,  606105819);
    b = ff(b, c, d, a, x[i+ 3], 22, -1044525330);
    a = ff(a, b, c, d, x[i+ 4], 7 , -176418897);
    d = ff(d, a, b, c, x[i+ 5], 12,  1200080426);
    c = ff(c, d, a, b, x[i+ 6], 17, -1473231341);
    b = ff(b, c, d, a, x[i+ 7], 22, -45705983);
    a = ff(a, b, c, d, x[i+ 8], 7 ,  1770035416);
    d = ff(d, a, b, c, x[i+ 9], 12, -1958414417);
    c = ff(c, d, a, b, x[i+10], 17, -42063);
    b = ff(b, c, d, a, x[i+11], 22, -1990404162);
    a = ff(a, b, c, d, x[i+12], 7 ,  1804603682);
    d = ff(d, a, b, c, x[i+13], 12, -40341101);
    c = ff(c, d, a, b, x[i+14], 17, -1502002290);
    b = ff(b, c, d, a, x[i+15], 22,  1236535329);    

    a = gg(a, b, c, d, x[i+ 1], 5 , -165796510);
    d = gg(d, a, b, c, x[i+ 6], 9 , -1069501632);
    c = gg(c, d, a, b, x[i+11], 14,  643717713);
    b = gg(b, c, d, a, x[i+ 0], 20, -373897302);
    a = gg(a, b, c, d, x[i+ 5], 5 , -701558691);
    d = gg(d, a, b, c, x[i+10], 9 ,  38016083);
    c = gg(c, d, a, b, x[i+15], 14, -660478335);
    b = gg(b, c, d, a, x[i+ 4], 20, -405537848);
    a = gg(a, b, c, d, x[i+ 9], 5 ,  568446438);
    d = gg(d, a, b, c, x[i+14], 9 , -1019803690);
    c = gg(c, d, a, b, x[i+ 3], 14, -187363961);
    b = gg(b, c, d, a, x[i+ 8], 20,  1163531501);
    a = gg(a, b, c, d, x[i+13], 5 , -1444681467);
    d = gg(d, a, b, c, x[i+ 2], 9 , -51403784);
    c = gg(c, d, a, b, x[i+ 7], 14,  1735328473);
    b = gg(b, c, d, a, x[i+12], 20, -1926607734);
    
    a = hh(a, b, c, d, x[i+ 5], 4 , -378558);
    d = hh(d, a, b, c, x[i+ 8], 11, -2022574463);
    c = hh(c, d, a, b, x[i+11], 16,  1839030562);
    b = hh(b, c, d, a, x[i+14], 23, -35309556);
    a = hh(a, b, c, d, x[i+ 1], 4 , -1530992060);
    d = hh(d, a, b, c, x[i+ 4], 11,  1272893353);
    c = hh(c, d, a, b, x[i+ 7], 16, -155497632);
    b = hh(b, c, d, a, x[i+10], 23, -1094730640);
    a = hh(a, b, c, d, x[i+13], 4 ,  681279174);
    d = hh(d, a, b, c, x[i+ 0], 11, -358537222);
    c = hh(c, d, a, b, x[i+ 3], 16, -722521979);
    b = hh(b, c, d, a, x[i+ 6], 23,  76029189);
    a = hh(a, b, c, d, x[i+ 9], 4 , -640364487);
    d = hh(d, a, b, c, x[i+12], 11, -421815835);
    c = hh(c, d, a, b, x[i+15], 16,  530742520);
    b = hh(b, c, d, a, x[i+ 2], 23, -995338651);

    a = ii(a, b, c, d, x[i+ 0], 6 , -198630844);
    d = ii(d, a, b, c, x[i+ 7], 10,  1126891415);
    c = ii(c, d, a, b, x[i+14], 15, -1416354905);
    b = ii(b, c, d, a, x[i+ 5], 21, -57434055);
    a = ii(a, b, c, d, x[i+12], 6 ,  1700485571);
    d = ii(d, a, b, c, x[i+ 3], 10, -1894986606);
    c = ii(c, d, a, b, x[i+10], 15, -1051523);
    b = ii(b, c, d, a, x[i+ 1], 21, -2054922799);
    a = ii(a, b, c, d, x[i+ 8], 6 ,  1873313359);
    d = ii(d, a, b, c, x[i+15], 10, -30611744);
    c = ii(c, d, a, b, x[i+ 6], 15, -1560198380);
    b = ii(b, c, d, a, x[i+13], 21,  1309151649);
    a = ii(a, b, c, d, x[i+ 4], 6 , -145523070);
    d = ii(d, a, b, c, x[i+11], 10, -1120210379);
    c = ii(c, d, a, b, x[i+ 2], 15,  718787259);
    b = ii(b, c, d, a, x[i+ 9], 21, -343485551);

    a = add(a, olda);
    b = add(b, oldb);
    c = add(c, oldc);
    d = add(d, oldd);
  }
  return rhex(a) + rhex(b) + rhex(c) + rhex(d);
}

/*function PassPhrase104(F) 
{

        var     pseed2 = "";
        Length2 = getObj("weppassphrase_id").value.length;

        for(p=0; p<64; p++) {
                tempCount = p % Length2;
                pseed2 += getObj("weppassphrase_id").value.substring(tempCount, tempCount+1);
        }
        md5Str = calcMD5(pseed2);

        getObj("key1_id").value = md5Str.substring(0, 26).toUpperCase();
        getObj("key2_id").value = md5Str.substring(0, 26).toUpperCase();
        getObj("key3_id").value = md5Str.substring(0, 26).toUpperCase();
        getObj("key4_id").value = md5Str.substring(0, 26).toUpperCase();
}*/

function PassPhrase104(passphraseStr, KEY1, KEY2, KEY3, KEY4) 
{

	var     pseed2 = "";
	Length2 = passphraseStr.value.length;

	for(p=0; p<64; p++) {
		tempCount = p % Length2;
		pseed2 += passphraseStr.value.substring(tempCount, tempCount+1);
	}
	md5Str = calcMD5(pseed2);

	KEY1.value = md5Str.substring(0, 26).toUpperCase();
	KEY2.value = md5Str.substring(0, 26).toUpperCase();
	KEY3.value = md5Str.substring(0, 26).toUpperCase();
	KEY4.value = md5Str.substring(0, 26).toUpperCase();
}

function PassPhrase104_ap(F) 
{

        var     pseed2 = "";
        Length2 = F.ap_sec_phrase.value.length;

        for(p=0; p<64; p++) {
                tempCount = p % Length2;
                pseed2 += F.ap_sec_phrase.value.substring(tempCount, tempCount+1);
        }
        md5Str = calcMD5(pseed2);

        F.ap_wepkey1.value = md5Str.substring(0, 26).toUpperCase();
        F.ap_wepkey2.value = md5Str.substring(0, 26).toUpperCase();
        F.ap_wepkey3.value = md5Str.substring(0, 26).toUpperCase();
        F.ap_wepkey4.value = md5Str.substring(0, 26).toUpperCase();
}
/*function clickgenerate(form)
{
	//form.time_stamp_dni.value=parent.index_ts;
	//if(getObj("weppassphrase_id").value.length == 0 )
	if(document.getElementsByName("weppassphrase")[0].value.length == 0 )
	{
		alert("$gene_phrase")
                return false;
	}
	//for(i=0;i<getObj("weppassphrase_id").value.length;i++)
    for(i=0;i<document.getElementsByName("weppassphrase")[0].value.length;i++)
         {
               // if(isValidChar_space(getObj("weppassphrase_id").value.charCodeAt(i))==false)
                if(isValidChar_space(document.getElementsByName("weppassphrase")[0].value.charCodeAt(i))==false)
                {
                        alert("$gene_phrase");
                        return false;
                }
        }			
    if(getObj("keylen_id").options[0].selected == true)
                PassPhrase40(form);
	else
                PassPhrase104(form);
	form.generate_flag.value=1;
}*/

function clickgenerate(form)
{

	if(form.passphraseStr.value.length == 0 )
	{
	//	alert("$gene_phrase")
			return false;
	} 
	for(i=0;i<form.passphraseStr.value.length;i++)
	{
		if(isValidChar_space(form.passphraseStr.value.charCodeAt(i))==false)
		{
			alert("$notallowpassp");
			return false;
		}
	}			
	if(form.wepenc.options[0].selected == true)
		PassPhrase40(form.passphraseStr, form.wepenc, form.wep_key_no, form.KEY1, form.KEY2, form.KEY3, form.KEY4);
	else
		PassPhrase104(form.passphraseStr, form.KEY1, form.KEY2, form.KEY3, form.KEY4);
	form.generate_flag.value=1;
}
function clickgenerate_a(form)
{
	if(form.passphraseStr_an.value.length == 0 )
	{
	//	alert("$gene_phrase")
			return false;
	} 
	for(i=0;i<form.passphraseStr_an.value.length;i++)
	{
		if(isValidChar_space(form.passphraseStr_an.value.charCodeAt(i))==false)
		{
			alert("$notallowpassp");
			return false;
		}
	}			
	if(form.Wepenc_an.options[0].selected == true)
		PassPhrase40(form.passphraseStr_an, form.Wepenc_an, form.wep_key_no_an, form.KEY1_an, form.KEY2_an, form.KEY3_an, form.KEY4_an);
	else
		PassPhrase104(form.passphraseStr_an, form.KEY1_an, form.KEY2_an, form.KEY3_an, form.KEY4_an);
	form.generate_flag.value=1;
}

function clickgenerate_ap(form)
{
        if(form.ap_sec_phrase.value.length == 0 )
        {
                alert(gene_phrase)
                return false;
        }
        for(i=0;i<form.ap_sec_phrase.value.length;i++)
         {
                if(isValidChar_space(form.ap_sec_phrase.value.charCodeAt(i))==false)
                {
                        alert(gene_phrase);
                        return false;
                }
        }
    if(form.ap_sec_keylen.value == 5)
                PassPhrase40_ap(form);
        else
                PassPhrase104_ap(form);
        form.ap_generate_flag.value=1;
}

function clickgenerate_client_manual(form)
{
        if(form.ap_weppassphrase.value.length == 0 )
        {
                alert(gene_phrase)
                return false;
        }
        for(i=0;i<form.ap_weppassphrase.value.length;i++)
         {
                if(isValidChar_space(form.ap_weppassphrase.value.charCodeAt(i))==false)
                {
                        alert(gene_phrase);
                        return false;
                }
        }
    if(form.ap_sec_keylen.options[0].selected == true)
                PassPhrase40_ap(form);
        else
                PassPhrase104_ap(form);
        form.ap_generate_flag.value=1;
}

function doPortChange(check)
{
	if(check == 0)
		location.href="forwarding.htm"
	else
		location.href="triggering.htm"
}

function setDisabled(OnOffFlag,formFields)
{
	for (var i = 1; i < setDisabled.arguments.length; i++)
		setDisabled.arguments[i].disabled = OnOffFlag;
}

function change_serv(cf)
{
	if( cf.login_type[2].selected == true )
		location.href="bas_pppoe.htm";
	else if( cf.login_type[0].selected == true )
		location.href="bas_pptp.htm";
	else if (cf.login_type[1].selected == true )
		location.href="bas_bpa.htm";	
	else if (cf.login_type[3].selected == true )
	        location.href="bas_mulpppoe.htm";
			 
		
}

function maccheck(mac_addr)
{
	if ( mac_addr.indexOf(":")==-1 && mac_addr.length=="12" )
	{
        var tmp_mac=mac_addr.substr(0,2)+":"+mac_addr.substr(2,2)+":"+mac_addr.substr(4,2)+":"+mac_addr.substr(6,2)+":"+mac_addr.substr(8,2)+":"+mac_addr.substr(10,2);
		mac_addr = tmp_mac;
	}
	mac_array=mac_addr.split(':');
	var mac11 = mac_array[0];
    	mac11 = mac11.substr(1,1);
	if((mac11=="1")||(mac11=="3")||(mac11=="5")||(mac11=="7")||(mac11=="9")||(mac11=="b")||(mac11=="d")||(mac11=="f")||(mac11=="B")||(mac11=="D")||(mac11=="F"))
    	{
		if(mac11=="1" &&   mac_array[0] == "11")
		{}
		else
		{
			alert("$invalid_mac");
        		return false;
		}
    	}
	if(mac_array.length!=6)
	{
		alert("$invalid_mac");
                return false;
	}
    	if(( mac_array[0]=="")||( mac_array[1]=="")||( mac_array[2]=="")||( mac_array[3]=="")||( mac_array[4]=="")||( mac_array[5]==""))
	{
		alert("$enter_full_mac");
		return false;
	}
    if((( mac_array[0]=="00")&&( mac_array[1]=="00")&&
        ( mac_array[2]=="00")&&( mac_array[3]=="00")&&
        ( mac_array[4]=="00")&&( mac_array[5]=="00"))||
		(( mac_array[0]=="11")&&( mac_array[1]=="11")&&
        ( mac_array[2]=="11")&&( mac_array[3]=="11")&&
        ( mac_array[4]=="11")&&( mac_array[5]=="11"))||
       (( mac_array[0]=="ff")&&( mac_array[1]=="ff")&&
        ( mac_array[2]=="ff")&&( mac_array[3]=="ff")&&
        ( mac_array[4]=="ff")&&( mac_array[5]=="ff"))||
       (( mac_array[0]=="FF")&&( mac_array[1]=="FF")&&
        ( mac_array[2]=="FF")&&( mac_array[3]=="FF")&&
        ( mac_array[4]=="FF")&&( mac_array[5]=="FF")))
	{
		alert("$invalid_mac");
		return false;
	}
	if(( mac_array[0].length!=2)||( mac_array[1].length!=2)||
	   ( mac_array[2].length!=2)||( mac_array[3].length!=2)||
           ( mac_array[4].length!=2)||( mac_array[5].length!=2))
	{
		alert("$invalid_mac");
		return false;
	}
	
	for(i=0;i<mac_addr.length;i++)
         {
                if(isValidMac(mac_addr.charAt(i))==false)
                {
                        alert("$invalid_mac");
                        return false;
                }
        }
	return true;
}

function setMAC(cf,this_mac)
{
	var dflag;
	if (cf.MACAssign[0].checked || cf.MACAssign[1].checked)
	{
		dflag = true;
		cf.this_mac.value=this_mac;
		setDisabled(dflag,cf.this_mac);
	}
	else
	{
		dflag = false;
		setDisabled(dflag,cf.this_mac);
		cf.this_mac.value=this_mac;
	}
}

function maccheck_wds(mac_addr,num,form)
{
	var return_num=0;
	//if(mac_addr==":::::")
	if(mac_addr=="")
		return 2;
	var mac_array=mac_addr.split(':');
	var mac11 = mac_array[0];
    	mac11 = mac11.substr(1,1);
    if((mac11=="1")||(mac11=="3")||(mac11=="5")||(mac11=="7")||
       (mac11=="9")||(mac11=="b")||(mac11=="d")||(mac11=="f")||
       (mac11=="B")||(mac11=="D")||(mac11=="F"))
	{	
		if( mac11 == "1" && mac_array[0]=="11")
		{}
		else
        		return 1;
	}
	if(mac_addr.length!=17 && mac_addr.length!=0)
    		return 1;
	if((mac_array[0]=="")||(mac_array[1]=="")||(mac_array[2]=="")||(mac_array[3]=="")|| (mac_array[4]=="")||(mac_array[5]==""))
	{
		if((mac_array[0]=="")&&(mac_array[1]=="")&&(mac_array[2]=="")&&(mac_array[3]=="")&& (mac_array[4]=="")&&(mac_array[5]==""))
                return 2;
		else
			return 1;
    }
	if(((mac_array[0]=="00")&&(mac_array[1]=="00")&&
        (mac_array[2]=="00")&&(mac_array[3]=="00")&&
        (mac_array[4]=="00")&&(mac_array[5]=="00"))||
       ((mac_array[0]=="ff")&&(mac_array[1]=="ff")&&
        (mac_array[2]=="ff")&&(mac_array[3]=="ff")&&
        (mac_array[4]=="ff")&&(mac_array[5]=="ff"))||
       ((mac_array[0]=="FF")&&(mac_array[1]=="FF")&&
        (mac_array[2]=="FF")&&(mac_array[3]=="FF")&&
        (mac_array[4]=="FF")&&(mac_array[5]=="FF")))
                return 1;
        if((mac_array[0].length!=2)||(mac_array[1].length!=2)||
           (mac_array[2].length!=2)||(mac_array[3].length!=2)||
       (mac_array[4].length!=2)||(mac_array[5].length!=2))
                return 1;
	for(i=0;i<mac_addr.length;i++)
                if(isValidMac(mac_addr.charAt(i))==false)
                        return 1;
	if( num != "" && num != 0 && num != 1 )
	{
		for( k=1;k<num;k++)
		{
			mac_str=eval('the_mac'+k);
			if( mac_str != "" && mac_str == mac_addr)
			{
				return 3;
			}
		}
	}	
		return return_num;
}

function change_sec_to_time(uptime)
{
	var sec=uptime;
	var sec=parseInt(sec);
//bug 24589:If the uptime is larger than 24 hours, please mention it as dd:hh:mm:ss, otherwise should be hh:mm:ss.
	var day_sec=sec%86400;
	if(day_sec!=sec)
		new_day=(sec-day_sec)/86400;
	else
		new_day=0;
	var hour_sec=day_sec%3600;
	if(hour_sec!=day_sec)
        	new_hour=(day_sec-hour_sec)/3600;
	else
        	new_hour=0;
	var min_sec=hour_sec%60;
	if(min_sec!=hour_sec)
        	new_min=(hour_sec-min_sec)/60;
	else
        	new_min=0;
	var new_sec=min_sec;
	new_day=new_day.toString();
	new_hour=new_hour.toString();
	new_min=new_min.toString();
	new_sec=new_sec.toString();
	if(new_day.length==1)
		new_day='0'+new_day;
	if(new_hour.length==1)
       		new_hour='0'+new_hour;
	if(new_min.length==1)
        	new_min='0'+new_min;
	if(new_sec.length==1)
        	new_sec='0'+new_sec;
	if(new_day == '00')
	{
		var new_time=new_hour+':'+new_min+':'+new_sec;
	}
	else
	{
		var new_time=new_day+':'+new_hour+':'+new_min+':'+new_sec;
	}

	return new_time;
}

function goTestApply()
{
	var winoptions = "width=640,height=480,menubar=yes,scrollbars=yes,toolbar=yes,status=yes,location=yes,resizable=yes"
	if( run_test == "test")
		window.open('testpage.htm',null,winoptions);
}

function isValidMac(digit)
{
        var macVals = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9","A", "B", "C", "D", "E", "F", "a", "b", "c", "d", "e", "f",":");
        var len = macVals.length;
        var i = 0;
        var ret = false;
        for ( i = 0; i < len; i++ )
        if ( digit == macVals[i] ) break;
        if ( i < len )
                ret = true;
                return ret;
}

function isValidHex(each_char)
{
	var macVals = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9","A", "B", "C", "D", "E", "F", "a", "b", "c", "d", "e", "f");
        var len = macVals.length;
        var i = 0;
        var ret = false;
        for ( i = 0; i < len; i++ )
        if ( each_char == macVals[i] ) break;
        if ( i < len )
                ret = true;
                return ret;
}

function isValidChar_space(each_char)
{
	if( each_char < 32 || each_char > 127)
		return false;
}

function isValidChar(each_char)
{
	if( each_char < 33 || each_char > 126)
		return false;
}

function isValidDdnsHost(each_char)
{
	if (!(((each_char>47) && (each_char<58))||(each_char==45)||(each_char==46)||((each_char>64) && (each_char<91))||((each_char>96) && (each_char<123)) || (each_char==8)||(each_char==0))) 
	return false;
}
function printPage()
{
    if (window.print)
        window.print();
    else
        //alert("Your browser doesn't support print function!");
        alert(browser_info);
}
function url_change_ornot ()
{
         var browser=eval ( '"' + top.location + '"' );
         if  ( lan_dhcp == 1)
         {
                 judge_browser(document.forms[0],browser);
         }
}
function judge_browser(cf,browser)
{
        if (browser.indexOf("mywifiext.net") > -1 )
        {
		cf.refresh_url.value="com";
                cf.refresh_page.value="client_setting";
                //parent.location.href="http://www.mywifiext.com/cgi-bin/index.htm";
        }
        else if (browser.indexOf("mywifiext.com")>-1 )
        {
                cf.refresh_url.value="net";
                cf.refresh_page.value="client_setting";
                //parent.location.href="http://mywifiext.net/cgi-bin/index.htm";
        }
        else
	{
		cf.refresh_url.value="manual_ip";
		cf.refresh_page.value="client_setting";
	}
}
function judge_browser_nochange(cf,browser)
{
        if (browser.indexOf("mywifiext.net") > -1 )
        {
                cf.refresh_url.value="net";
                cf.refresh_page.value="client_setting";
                //parent.location.href="http://www.mywifiext.com/cgi-bin/index.htm";
        }
        else if (browser.indexOf("mywifiext.com")>-1 )
        {
                cf.refresh_url.value="com";
                cf.refresh_page.value="client_setting";
                //parent.location.href="http://mywifiext.net/cgi-bin/index.htm";
        }
        else
        {
                cf.refresh_url.value="manual_ip";
                cf.refresh_page.value="client_setting";
        }
}

function top_left_nolink()
{
	if( parent.multi_lang_router_flag == 1 )
	{
		parent.topframe.document.forms[0].lang_avi.disabled = true;
		//parent.topframe.document.forms[0].apply.disabled = true;
	}
}
