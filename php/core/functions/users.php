<?php
function change_password ($user_id, $password)
{
    $user_id = (int)$user_id;
	$password = check_for_script_tags($password);
	$password = convert_from_html($password);
	$password = sanitize($password);
	mysql_query("UPDATE `users` SET `password` = '$password' WHERE `user_id` = $user_id");
}
function register_user($register_data)
{
    array_walk($register_data, 'array_check_for_script_tags');
	array_walk($register_data, 'array_convert_from_html');
    array_walk($register_data, 'array_sanitize');
	$fields = '`' . implode('`, `', array_keys($register_data)) . '`';
	$data ='\'' . implode('\', \'', $register_data) . '\'';
	mysql_query("INSERT INTO `users` ($fields) VALUES ($data)");
}
function user_data($user_id)
{
    $data = array();
	$user_id = (int)$user_id;
	$func_num_args = func_num_args();
	$func_get_args = func_get_args();
	if ($func_num_args > 1)
	{
	   unset($func_get_args[0]);
	   $fields = '`' . implode('`, `', $func_get_args) . '`';
	   $data = mysql_fetch_assoc(mysql_query("SELECT $fields FROM `users` WHERE `user_id` = $user_id"));
	   return $data;
	}
}
function logged_in()
{
    return (isset($_SESSION['user_id'])) ? true : false;
}
function user_exists($username)
{
  $username = check_for_script_tags($username);
  $username = convert_from_html($username);
  $username = sanitize($username);
  $query = mysql_query("SELECT COUNT(`user_id`) FROM `users` WHERE `username` = '$username'");
  return (mysql_result($query, 0) == 1) ? true : false;
}
function email_exists($email)
{
  $email = check_for_script_tags($email);
  $email = convert_from_html($email);
  $email = sanitize($email);
  $query = mysql_query("SELECT COUNT(`user_id`) FROM `users` WHERE `email` = '$email'");
  return (mysql_result($query, 0) == 1) ? true : false;
}
function user_active($username)
{
  $username = check_for_script_tags($username);
  $username = convert_from_html($username);
  $username = sanitize($username);
  $query = mysql_query("SELECT COUNT(`user_id`) FROM `users` WHERE `username` = '$username' AND `active` = 1");
  return (mysql_result($query, 0) == 1) ? true : false;
}
function user_id_from_username($username)
{
    $username = check_for_script_tags($username);
    $username = convert_from_html($username);
    $username = sanitize($username);
	return mysql_result(mysql_query("SELECT `user_id` FROM `users` WHERE `username` = '$username'"), 0, 'user_id');
}
function login($username, $password)
{
   $user_id = user_id_from_username($username);
   $username = check_for_script_tags($username);
   $username = convert_from_html($username);
   $username = sanitize($username);
   $password = check_for_script_tags($password);
   $password = convert_from_html($password);
   $password = sanitize($password);
   $query1 = mysql_query("SELECT COUNT(`user_id`) FROM `users` WHERE `username` = '$username' AND `password` = '$password'") or die(mysql_error(0));
   return (mysql_result($query1, 0) == 1) ? $user_id : false;
}
?>