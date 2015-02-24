function check_wlacl_add(cf,flag)
{
	if( array_num == 64 && flag=="add")
	{
		alert("$acl_length_64");
		return false;
	}
	if(cf.device.value=="")
    	{
        	alert("$device_name_null");
        	return false;
    	}	
	for(i=0;i<cf.device.value.length;i++)
        {         
                if(isValidChar(cf.device.value.charCodeAt(i))==false)
                {
                        alert("$device_name_error");
                        return false;
                }
        }
	for(i=1;i<=array_num;i++)
    {
		var str = eval ( 'wlaclArray' + i );
		var tmp_str=change_blank(str); 
		var each_info=tmp_str.split(' ');
		if (flag == 'add')
		{
		each_info[0]=each_info[0].replace(/<lt;/g,'<');
                each_info[0]=each_info[0].replace(/>gt;/g,'>');		
		if(each_info[0].toLowerCase()==cf.device.value.toLowerCase())
        	{
            	alert("$device_name_dup");
            	return false;
        	}
		}
		else
		{
			if( each_info[0].toLowerCase()==cf.device.value.toLowerCase() && i != select_edit )
	        {
	        	alert("$device_name_dup");
				return false;
			}
		}
    }
	    if(cf.this_mac.value.length==12 && cf.this_mac.value.indexOf(":")==-1)
	    {
	        var mac=cf.this_mac.value; 
	        cf.this_mac.value=mac.substr(0,2)+":"+mac.substr(2,2)+":"+mac.substr(4,2)+":"+mac.substr(6,2)+":"+mac.substr(8,2)+":"+mac.substr(10,2);
	        
	    }
		else if ( cf.this_mac.value.split("-").length == 6 )
		{
			var tmp_mac = cf.this_mac.value.replace(/-/g,":");
			cf.this_mac.value=tmp_mac;
		}	

	if(maccheck(cf.this_mac.value) == false)
        	return false;
	for(i=1;i<=array_num;i++)
    {
		var str = eval ( 'wlaclArray' + i );
		var tmp_str=change_blank(str); 
		var each_info=tmp_str.split(' ');
		if (flag == 'add')
		{
        	if(each_info[1].toLowerCase() == cf.this_mac.value.toLowerCase())
        	{
            		alert("$mac_dup");
            		return false;
        	}
		}
		else
		{
			if( each_info[1].toLowerCase() == cf.this_mac.value.toLowerCase() && i!= select_edit )
			{
				alert("$mac_dup");
				return false;
			}
		}
    }	
	return true;
}

function check_wlacl_edit(cf,flag)
{
if(cf.device.value=="")
        {
            alert("$device_name_null");
            return false;
        }
    for(i=0;i<cf.device.value.length;i++)
        {
                if(isValidChar(cf.device.value.charCodeAt(i))==false)
                {
                        alert("$device_name_error");
                        return false;
                }
        }
    for(i=1;i<=array_num;i++)
    {
        var str = eval ( 'wlaclArray' + i );
		var tmp_str=change_blank(str); 
		var each_info=tmp_str.split(' ');
        if (flag == 'add')
        {
            if(each_info[0]==cf.device.value)
            {
                alert("$device_name_dup");
                return false;
            }
        }
        else
        {
            if( each_info[0]==cf.device.value && i != select_edit )
            {
                alert("$device_name_dup");
                return false;
            }
        }
    }
	    if(cf.this_mac.value.length==12 && cf.this_mac.value.indexOf(":")==-1)
	    {
	        var mac=cf.this_mac.value; 
	        cf.this_mac.value=mac.substr(0,2)+":"+mac.substr(2,2)+":"+mac.substr(4,2)+":"+mac.substr(6,2)+":"+mac.substr(8,2)+":"+mac.substr(10,2);
	        
	    }
		else if ( cf.this_mac.value.split("-").length == 6 )
		{
			var tmp_mac = cf.this_mac.value.replace(/-/g,":");
			cf.this_mac.value=tmp_mac;
		}	
    if(maccheck(cf.this_mac.value) == false)
            return false;
    for(i=1;i<=array_num;i++)
    {
        var str = eval ( 'wlaclArray' + i );
		var tmp_str=change_blank(str); 
		var each_info=tmp_str.split(' ');
        if (flag == 'add')
        {
            if(each_info[1]==cf.this_mac.value)
            {
                    alert("$mac_dup");
                    return false;
            }
        }
        else
        {
            if( each_info[1] == cf.this_mac.value && i!= select_edit )
            {
                alert("$mac_dup");
                return false;
            }
        }
    }
    return true;
}

function check_wlacl_editnum(cf)
{
	if( array_num == "" )
	{
		alert("$no_serv_edit");
		return false;
	}
	var count_select=0;
	var select_num;
	if( array_num == 1)
	{
		if(cf.select_wlacl.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<array_num;i++)
		if(cf.select_wlacl[i].checked == true)
		{
			count_select++;
			select_num=i+1;
		}
	if(count_select==0)
	{
		alert("$select_serv_edit");
		return false;
	}
	else
	{
		cf.select_edit.value=select_num;
		cf.submit_flag.value="wlacl_editnum";
		cf.action="/apply.cgi?/wlacl_edit.htm timestamp="+ts;
		return true;
	}
}

function check_wlacl_del(cf)
{
	if( array_num == "" )
	{
		alert("$no_serv_del");
		return false;
	}
	var count_select=0;
	var select_num;
	if( array_num == 1)
	{
		if(cf.select_wlacl.checked == true)
		{
			count_select++;
			select_num=1;
		}
	}
	else for(i=0;i<array_num;i++)
		if(cf.select_wlacl[i].checked == true)
		{
			count_select++;
			select_num=i+1;
		}
	if(count_select==0)
	{
		alert("$select_serv_del");
		return false;
	}
	else
	{
		if( cf.accessLimit.checked == true && array_num == 1)
		{
			if(!confirm("$wps_warning4"))
				return false;
		}
		cf.select_del.value=select_num;
		cf.submit_flag.value="wlacl_del";
		cf.action="/apply.cgi?/wlacl.htm timestamp="+ts;
		return true;
	}
}

function check_wlacl_apply(cf)
{
	if( cf.accessLimit.checked == true )
		cf.wl_access_ctrl_on.value=1;
	else
		cf.wl_access_ctrl_on.value=0;
		
	if( cf.wl_access_ctrl_on.value == 1 && array_num == 0)
	{
		if(!confirm("$wps_warning4"))
			return false;
	}
	
		cf.submit_flag.value="wlacl_apply";
		cf.action="/apply.cgi?/wlacl.htm timestamp="+ts;
	return true;
}

function mac_data_select(num)
{
	var cf = document.forms[0];
	cf.device.value = sta_name_array[num];
	cf.this_mac.value=sta_mac_array[num];
}
