<?php
include 'core/init.php';
logged_in_redirect();
if (empty($_POST) === false)
{
    $username = $_POST['username'];
	$password = $_POST['password'];
	if (empty($username) === true || empty($password) === true)
	{
	   $errors[] = 'You need to enter your username and password.';
	}
	else if (user_exists($username) === false)
	{
	   $errors[] = 'You have to register first.';
	}
	else if (user_active($username) === false)
	{
	   $errors[] = 'You have not activated your account.';
	}
	else if (strlen($username) > 20)
	{
	    $errors[] = 'Your username is too long.';
	}
	else if (strlen($password) > 30)
	{
	    $errors[] = 'Your password is too long.';
	}
	else
	{
	   $login = login($username, $password);
	   if ($login === false)
	   {
	       $errors[] = 'This username and password combination is incorrect.';
	   }
	   else
	   {
	         $_SESSION['user_id'] = $login;
			 header('Location: index.php');
			 exit();
	   }
	}
	print_r($errors);
}
else
{
   $errors[] = 'No data has been entered.';
} 
include 'includes/overall/header.php';
if (empty($errors) === false)
{
?>
<h2> We tried to log you in, but... </h2>
<?php
echo output_errors($errors);
}
include 'includes/overall/footer.php';
?>