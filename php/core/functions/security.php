<?php
function check_for_script_tags($data)
{
   return strip_tags($data);
}
function convert_from_html($data)
{
   return htmlentities($data);
}
function array_check_for_script_tags(&$item)
{
   $item = strip_tags($item);
}
function array_convert_from_html(&$item)
{
   $item = htmlentities($item);
}
?>