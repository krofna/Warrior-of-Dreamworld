<?php
$connection_problem = "Sorry, we are currently having connection problems. Please try again later.";
mysql_connect('localhost', 'root', '') or die($connection_problem);
mysql_select_db('lr') or die($connection_problem);
?>