function lanip_change(cf)
{
	var addr_array = new Array();
	cf.ether_ipaddr.value=cf.ether_ip1.value+'.'+cf.ether_ip2.value+'.'+cf.ether_ip3.value+'.'+cf.ether_ip4.value;
	addr_array = cf.ether_ipaddr.value.split('.');
	cf.ip_start1.value=cf.ip_end1.value=addr_array[0];
	cf.ip_start2.value=cf.ip_end2.value= addr_array[1];
	cf.ip_start3.value=cf.ip_end3.value=addr_array[2];	
}


function setIP(cf)
{
	var dflag = cf.LANAssign[0].checked;
	setDisabled(dflag,cf.ether_ip1,cf.ether_ip2,cf.ether_ip3,cf.ether_ip4,cf.ether_mask1,cf.ether_mask2,cf.ether_mask3,cf.ether_mask4,cf.ether_gtw1,cf.ether_gtw2,cf.ether_gtw3,cf.ether_gtw4);

	setDisabled(dflag,cf.IPAssign[0],cf.ip_start1,cf.ip_start2,cf.ip_start3,cf.ip_start4,cf.ip_end1,cf.ip_end2,cf.ip_end3,cf.ip_end4);

	var dflag_2 = cf.LANAssign[1].checked;
	setDisabled(dflag_2,cf.IPAssign[2]);

	if(dflag == true && cf.IPAssign[0].checked == true)
		cf.IPAssign[1].checked = true;
	if(dflag_2 == true && cf.IPAssign[2].checked == true)
		cf.IPAssign[1].checked = true;

	set_able(cf);
	set_auto(cf);
	set_disable(cf);
	
	DisableFixedIP = dflag;
}

function setDNS(cf)
{
	var dflag = cf.LANAssign[0].checked;
	setDisabled(dflag,cf.ether_dnsp1,cf.ether_dnsp2,cf.ether_dnsp3,cf.ether_dnsp4);

	DisableFixedDNS = dflag;
}

function set_able(cf)
{
		var dflag = cf.IPAssign[0].checked;
		setDisabled(dflag,cf.ip_start4,cf.ip_end4);		
}

function set_disable(cf)
{
		var dflag = cf.IPAssign[0].checked;
		setDisabled(!dflag,cf.ip_start1,cf.ip_start2,cf.ip_start3,cf.ip_start4,cf.ip_end1,cf.ip_end2,cf.ip_end3,cf.ip_end4);
}

function set_auto(cf)
{
		var dflag = cf.IPAssign[2].checked;
		setDisabled(dflag,cf.ip_start4,cf.ip_end4);
}

function check_wizard_dhcp(check)
{
	cf=document.forms[0];
/*	
	if(cf.ac_name.value=="")
	{
		alert(account_name_null);
		return false;
	}
	for(i=0;i<cf.ac_name.value.length;i++)
        {
                if(isValidChar_space(cf.ac_name.value.charCodeAt(i))==false)
                {
                        alert(acname_not_allowed);
                        return false;
                }
        }
	for(i=0;i<cf.do_name.value.length;i++)
        {
                if(isValidChar_space(cf.do_name.value.charCodeAt(i))==false)
                {
                        alert(doname_not_allowed);
                        return false;
                }
        }
*/
	if (check == 1)
		cf.run_test.value="test"
	else
		cf.run_test.value="no"
	return true;
}

function check_static_ip_mask_gtw()
{
	cf=document.forms[0];
	cf.ether_ipaddr.value=cf.ether_ip1.value+'.'+cf.ether_ip2.value+'.'+cf.ether_ip3.value+'.'+cf.ether_ip4.value;
	cf.ether_subnet.value=cf.ether_mask1.value+'.'+cf.ether_mask2.value+'.'+cf.ether_mask3.value+'.'+cf.ether_mask4.value;
	cf.ether_gateway.value=cf.ether_gtw1.value+'.'+cf.ether_gtw2.value+'.'+cf.ether_gtw3.value+'.'+cf.ether_gtw4.value;
	if(checkipaddr(cf.ether_ipaddr.value)==false || is_sub_or_broad(cf.ether_ipaddr.value, cf.ether_ipaddr.value, cf.ether_subnet.value) == false)
	{
		alert("$invalid_ip");
		return false;
	}
	if(checksubnet(cf.ether_subnet.value)==false)
	{
		alert("$invalid_mask");
		return false;
	}
	if(checkgateway(cf.ether_gateway.value)==false)
	{
		alert("$invalid_gateway");
		return false;
	}
	/*if(isGateway(cf.ether_ipaddr.value,cf.ether_subnet.value,cf.ether_gateway.value)==false)
	{
		alert("$invalid_gateway");
		return false;		
	}*/
	if( isSameIp(cf.ether_ipaddr.value, cf.ether_gateway.value) == true )
	{
		alert("$invalid_gateway");
		return false;
	}
	/*if(isSameSubNet(cf.ether_ipaddr.value,cf.ether_subnet.value,cf.ether_gateway.value,cf.ether_subnet.value) == false)
	{
		alert("$same_subnet_ip_gtw");
		return false;
	}*/
	return true;
}

function check_static_dns()
{
	var cf=document.forms[0];
	cf.ether_dnsaddr1.value=cf.ether_dnsp1.value+'.'+cf.ether_dnsp2.value+'.'+cf.ether_dnsp3.value+'.'+cf.ether_dnsp4.value;
	//cf.ether_dnsaddr2.value=cf.ether_dnss1.value+'.'+cf.ether_dnss2.value+'.'+cf.ether_dnss3.value+'.'+cf.ether_dnss4.value;
	if(checkipaddr(cf.ether_dnsaddr1.value)==false)
	{
		alert("$invalid_primary_dns");
		return false;
	}
	/*if(cf.ether_dnsaddr2.value=="...")
		cf.ether_dnsaddr2.value="";
	if(cf.ether_dnsaddr2.value!="")
	{
		if(checkipaddr(cf.ether_dnsaddr2.value)==false)
		{   
		    if(cf.ether_dnsaddr2.value=="0.0.0.0")
		    return true;
			else
			alert(invalid_second_dns);
			return false;
		}
	}*/
	return true;
}

function check_ether_samesubnet()
{
	var cf=document.forms[0];
        cf.ether_ipaddr.value=cf.ether_ip1.value+'.'+cf.ether_ip2.value+'.'+cf.ether_ip3.value+'.'+cf.ether_ip4.value;
        cf.ether_subnet.value=cf.ether_mask1.value+'.'+cf.ether_mask2.value+'.'+cf.ether_mask3.value+'.'+cf.ether_mask4.value;
        cf.ether_gateway.value=cf.ether_gtw1.value+'.'+cf.ether_gtw2.value+'.'+cf.ether_gtw3.value+'.'+cf.ether_gtw4.value;
	return true;
}

function check_wizard_static(check)
{
	var cf=document.forms[0];
	if(check_static_ip_mask_gtw()==false)
		return false;
	if(check_ether_samesubnet()==false)
		return false;	
	if(check_static_dns()==false)
		return false;		
	if (check == 1)
		cf.run_test.value="test"
	else
		cf.run_test.value="no"	
	return true;
}

function check_clientNumber(cf)
{
	cf.ether_subnet.value=cf.ether_mask1.value+'.'+cf.ether_mask2.value+'.'+cf.ether_mask3.value+'.'+cf.ether_mask4.value;
	var ether_mask=cf.ether_subnet.value;
	cf.ether_ipaddr.value=cf.ether_ip1.value+'.'+cf.ether_ip2.value+'.'+cf.ether_ip3.value+'.'+cf.ether_ip4.value;
	var ether_ip=cf.ether_ipaddr.value;
	var mask_array=ether_mask.split('.');
	var ether_array=ether_ip.split('.');
	var netmask = parseInt(mask_array[3], 10);
	var net_number = 256 / (256 - netmask);
	var client = 255-netmask;
	var localip = parseInt(ether_array[3],10);
	var net_start = (netmask & localip) + 1;
	var net_end = (net_start + client) - 1;
	if (localip == (net_start-1)) {
		cf.ether_ip.focus();
		return false;
	}
	if (localip >= (net_end)) {
		cf.ether_ip.focus();
		return false;
	}
	if ((parseInt(cf.ip_start4.value,10) < net_start) || (parseInt(cf.ip_start4.value,10) >= net_end)) {
		cf.ip_start4.focus();
		return false;
	}
	if ((parseInt(cf.ip_end4.value,10) < net_start) || (parseInt(cf.ip_end4.value,10) >= net_end)) {
		cf.ip_end4.focus();
		return false;
	}
	return true;
}

function check_ether(cf,check)
{
    cf.ether_ipaddr.value=cf.ether_ip1.value+'.'+cf.ether_ip2.value+'.'+cf.ether_ip3.value+'.'+cf.ether_ip4.value;
    cf.ether_subnet.value=cf.ether_mask1.value+'.'+cf.ether_mask2.value+'.'+cf.ether_mask3.value+'.'+cf.ether_mask4.value;
	cf.ether_gateway.value=cf.ether_gtw1.value+'.'+cf.ether_gtw2.value+'.'+cf.ether_gtw3.value+'.'+cf.ether_gtw4.value;
	cf.ether_dnsaddr1.value=cf.ether_dnsp1.value+'.'+cf.ether_dnsp2.value+'.'+cf.ether_dnsp3.value+'.'+cf.ether_dnsp4.value;

		cf.dh_start.value=cf.ip_start1.value+'.'+cf.ip_start2.value+'.'+cf.ip_start3.value+'.'+cf.ip_start4.value;
		cf.dh_end.value=cf.ip_end1.value+'.'+cf.ip_end2.value+'.'+cf.ip_end3.value+'.'+cf.ip_end4.value;
	if((cf.ether_ipaddr.value=="0.0.0.0")||(cf.ether_ipaddr.value=="255.255.255.255"))
	{
		alert("$invalid_ip");
		return false;
	}
	if((cf.ether_ip1.value=="0")||(cf.ether_ip1.value=="127")||cf.ether_ip1.value>223)
	{
		alert("$invalid_ip");
		return false;
	}
	if((cf.ether_ip1.value<0)||(cf.ether_ip2.value<0)||(cf.ether_ip3.value<0))
	{
		alert("$invalid_ip");
		return false;
	}
	if((cf.ether_ip1.value>255)||(cf.ether_ip2.value>255)||(cf.ether_ip3.value>255))
	{
		alert("$invalid_ip");
		return false;
	}
	if((cf.ether_ip4.value<=0)||(cf.ether_ip4.value>=255))
	{
		alert("$invalid_ip");
		return false;
	}
	if(cf.LANAssign[1].checked == true)
	{
		if(check_static_ip_mask_gtw()==false)
			return false;
	}
	
	  if(cf.ether_dnsaddr1.value!="")
        {
                if(checkipaddr(cf.ether_dnsaddr1.value)==false)
                {
                        alert("$invalid_primary_dns");
                        return false;
                }
        }
	if(cf.IPAssign[0].checked == true)
	{
		cf.IPAssign.value=1;
		if( checkipaddr(cf.dh_start.value)== false )
		{
			alert("$invalid_dhcp_startip");
			return false;
		}
		cf.dh_start.value = address_parseInt(cf.dh_start.value);
		if( checkipaddr(cf.dh_end.value)== false )
		{
			alert("$invalid_dhcp_endip");
			return false;	
		}
		cf.dh_end.value = address_parseInt(cf.dh_end.value);
		if(parseInt(cf.ip_start4.value,10)>parseInt(cf.ip_end4.value,10))
		{
			alert("$invalid_dhcp_startendip");
			return false;
		}
		if(!check_clientNumber(cf))
		{	
			alert("$invalid_dhcp_startendip");
			return false;
		}
		cf.ether_ipaddr.value = address_parseInt(cf.ether_ipaddr.value);
		cf.ether_subnet.value = address_parseInt(cf.ether_subnet.value);
		if(isSameSubNet(cf.ether_ipaddr.value,cf.ether_subnet.value,cf.dh_start.value,cf.ether_subnet.value) == false)
		{
			alert("$same_subnet_ip_dhcpstart");
			return false;
		}
		if(isSameSubNet(cf.ether_ipaddr.value,cf.ether_subnet.value,cf.dh_end.value,cf.ether_subnet.value) == false)
		{
			alert("$same_subnet_ip_dhcpend");
			return false
		}
	}
/*	else
	{
		cf.IPAssign.value=0;
	}*/
	if(cf.LANAssign[0].checked == true)
	{
		cf.LANAssign.value=1;		
	}
	else
	{
		cf.LANAssign.value=0;
		if((cf.ether_subnet.value=="0.0.0.0")||(cf.ether_subnet.value=="255.255.255.255")) 
	    {
		  alert("$invalid_mask");
		  return false;
		}
/*		if(checkgateway(cf.ether_gateway.value)==false)
		{
			alert("$invalid_gateway");
			return false;
		}
		if(isSameSubNet(cf.ether_ipaddr.value,cf.ether_subnet.value,cf.ether_gateway.value,cf.ether_subnet.value) == false)
		{
			alert("$same_subnet_ip_gtw");
			return false;
		}
		if(check_wizard_dhcp(check)==false)
			return false;

		if(check_static_ip_mask_gtw()==false)
			return false;

		if(check_static_dns()==false)
			return false;
*/
		if(isSameIp(cf.ether_ipaddr.value,old_lan_ip)==false)
        {
                var askstr="$changelanip"+cf.ether_ipaddr.value+" ?";
				if(!confirm(askstr))
                        return false;
                alert("$changelanip_renew");
                cf.change_ip_flag.value=1;
                cf.action="/apply.cgi?/welcomeok.htm timestamp="+ts;
				return true;
        }

	}
	return true;
}

function check_dhcpd(cf,check)
{
   cf.dh_start.value=cf.ip_start1.value+'.'+cf.ip_start2.value+'.'+cf.ip_start3.value+'.'+cf.ip_start4.value;
	cf.dh_end.value=cf.ip_end1.value+'.'+cf.ip_end2.value+'.'+cf.ip_end3.value+'.'+cf.ip_end4.value;
	
	if(cf.IPAssign[0].checked == true)
	{
		cf.IPAssign.value=1;		
	}
	else
	{
		cf.IPAssign.value=0;
	}		
	return true;
}

function check_welcome_dhcp()
{
	var cf = document.forms[0];
	//parent.welcome_wan_type=2;
	location.href="ca_05_SSID.htm";
}

function check_welcome_static()
{
	var cf = document.forms[0];
        cf.ether_ipaddr.value=cf.ether_ip1.value+'.'+cf.ether_ip2.value+'.'+cf.ether_ip3.value+'.'+cf.ether_ip4.value;
        cf.ether_subnet.value=cf.ether_mask1.value+'.'+cf.ether_mask2.value+'.'+cf.ether_mask3.value+'.'+cf.ether_mask4.value;
        cf.ether_gateway.value=cf.ether_gtw1.value+'.'+cf.ether_gtw2.value+'.'+cf.ether_gtw3.value+'.'+cf.ether_gtw4.value;
        cf.ether_dnsaddr1.value=cf.ether_dnsp1.value+'.'+cf.ether_dnsp2.value+'.'+cf.ether_dnsp3.value+'.'+cf.ether_dnsp4.value;
        cf.ether_dnsaddr2.value=cf.ether_dnss1.value+'.'+cf.ether_dnss2.value+'.'+cf.ether_dnss3.value+'.'+cf.ether_dnss4.value;
	if(cf.ether_dnsaddr2.value=="...")
		cf.ether_dnsaddr2.value="";
	if(check_wizard_static(0)==false)
			return false;	
/*			
	parent.welcome_wan_type=1;
	parent.static_ip=cf.ether_ipaddr.value;
	parent.static_subnet=cf.ether_subnet.value;
	parent.static_gateway=cf.ether_gateway.value;
	parent.static_dns1=cf.ether_dnsaddr1.value;
	parent.static_dns2=cf.ether_dnsaddr2.value;*/
	location.href="ca_05_SSID.htm";
}
