<?php
include 'core/init.php';
logged_in_redirect();
include 'includes/overall/header.php';
if (empty($_POST) === false)
{
    $required_fields = array('username', 'password', 'password_again', 'email', 'email_again');
	foreach($_POST as $key => $value)
	{
	    if (empty($value) && in_array($key, $required_fields) === true)
		{
		     $errors[] = 'All fields are required!';
			 break 1;
		}
	}
	if (empty($errors))
	{
	    if (user_exists($_POST['username']) === true)
		{
               $errors[] = 'Sorry, the username \'' .convert_from_html($_POST['username']). '\' is already taken.';	
		}
		if (preg_match("/\\s/", $_POST['username']) == true)
		{
		       $errors[] = 'Your username must not contain any spaces.';
		}
		if (strlen($_POST['password']) < 6)
		{
		       $errors[] = 'Your password must be at least 6 characters.';
 		}
		if (strlen($_POST['password']) > 30)
		{
		       $errors[] = 'Your password must not be longer than 30 characters.';
 		}
		if (strlen($_POST['username']) < 4)
		{
		       $errors[] = 'Your username must be at least 4 characters.';
 		}
		if (strlen($_POST['username']) > 20)
		{
		       $errors[] = 'Your username must not be longer than 20 characters.';
 		}
		if ($_POST['password'] !== $_POST['password_again'])
		{
		       $errors[] = 'Your passwords do not match.';
 		}
		if (filter_var($_POST['email'], FILTER_VALIDATE_EMAIL === false))
		{
		        $errors[] = 'A valid e-mail adress is required.';
		}
		if ($_POST['email'] !== $_POST['email_again'])
		{
		       $errors[] = 'Your e-mails do not match.';
 		}
		if (email_exists($_POST['email']) === true)
		{
		        $errors[] = 'Sorry, the e-mail \'' .convert_from_html($_POST['email']). '\' is already in use.';	
		}
	}
}

?>
		<h1> Register </h1>
		<?php
		if (isset($_GET['success']) && empty ($_GET['success']))
        {
             echo 'You have been registered successfully.';
        }	
        else
        {		
		if (empty($_POST) === false && empty($errors) === true)
		{
		     $register_data = array('username' => $_POST['username'], 'password' => $_POST['password'], 'email' => $_POST['email']);
			 register_user($register_data);
			 header('Location : register.php?success');
			 exit();
		}
		else if (empty($errors) === false)
		{
		      echo output_errors($errors);
		}
		?>
		<form action="" method="post">
		    <ul>
			  <li> 
			      Username : <br/>
                  <input type="text" name="username" />				  
			  </li> 
			  <li>
                   Password : <br/>
                   <input type="password" name="password" />				   
			  </li> 
			  <li>
                   Repeat password : <br/>
                   <input type="password" name="password_again" />				   
			  </li> 
			  <li>
                   E-mail : <br/>
                   <input type="text" name="email" />				   
			  </li> 
			  <li>
                   Repeat e-mail : <br/>
                   <input type="text" name="email_again" />				   
			  </li>
			  <li>
                   <input type="submit" value="Submit"/>				   
			  </li> 
			</ul>
		</form>
<?php
         }
include 'includes/overall/footer.php';
?>