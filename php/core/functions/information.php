<?php
function getBrowser()
{
    $browser = get_browser(null, true);
	return $browser;
}
function getOS()
{
    $operatingsystem = PHP_OS;
	return $operatingsystem;
}
function getPHPVersion()
{
   $version = PHP_VERSION;
   return $version;
}
?>