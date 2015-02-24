function check_ipv6_dhcp(cf)
{
	for(i=0;i<cf.userClass.value.length;i++)
	{
		if(isValidChar(cf.userClass.value.charCodeAt(i))==false)
		{
			alert("$acname_not_allowed");
			return false;
		}
	}

	for(i=0;i<cf.domainName.value.length;i++)
	{
		if(isValidDdnsHost(cf.domainName.value.charCodeAt(i))==false)
		{
			alert("$doname_not_allowed");
			return false;
		}
	}

	/*domain name must start with a letter, end with a letter or digit.*/
	if(cf.domainName.value != ""
		&& (isValidLetter(cf.domainName.value.charCodeAt(0))==false ||isValidNumLetter(cf.domainName.value.charCodeAt(cf.domainName.value.length-1))==false))
	{
		alert("$doname_not_allowed");
		return false;
	}

	return true;
}
