#Install MySQL 5.6 dependencies
echo "Installing MySQL 5.6 dependencies...";
sudo apt-get install -y libaio1 > /dev/null 2>&1;

#Download MySQL 5.6 installer
echo "Downloading MySQL 5.6 package...";
wget "http://dev.mysql.com/get/Downloads/MySQL-5.6/mysql-5.6.15-debian6.0-x86_64.deb" -O /tmp/mysql-5.6.15-debian6.0-x86_64.deb > /dev/null 2>&1;

#Add mysql user
echo "Creating mysql user..."
sudo useradd mysql > /dev/null 2>&1;

#Install MySQL 5.6 debian package
echo "Installing MySQL 5.6...";
sudo dpkg -i  /tmp/mysql-5.6.15-debian6.0-x86_64.deb > /dev/null 2>&1;
sudo rm -rf /tmp/mysql-5.6.15-debian6.0-x86_64.deb > /dev/null 2>&1;

#Remove additional my.cnf (we should rely on /etc/my.cnf)
echo "Removing additional my.cnf files...";
sudo rm -rf /opt/mysql/server-5.6/my.cnf > /dev/null 2>&1;

#Install mysql database
echo "Creating mysql database..."
sudo /opt/mysql/server-5.6/scripts/mysql_install_db > /dev/null 2>&1;

#Force mysql user to take ownership of mysql folder
echo "Configuring mysql folder permissions...";
sudo chown -R mysql.mysql /opt/mysql/ > /dev/null 2>&1;

#Add MySQL binaries path to environment path
echo "Adding MySQL binaries to PATH environment variable..."
echo "export PATH=$PATH:/opt/mysql/server-5.6/bin" >> /etc/profile > /dev/null 2>&1;
source /etc/profile > /dev/null 2>&1;

#Set MySQL root password (development)
echo "Setting MySQL root user password (development)...";
mysqladmin -u root password development > /dev/null 2>&1;

#Copy the MySQL service files to /etc/init.d/ and enable them.
echo "Configuring MySQL as a system daemon...";
sudo cp /opt/mysql/server-5.6/support-files/mysql.server /etc/init.d/ > /dev/null 2>&1;
sudo update-rc.d mysql.server defaults > /dev/null 2>&1;

#Start MySQL service
echo "Starting MySQL server...";
sudo /etc/init.d/mysql.server start > /dev/null 2>&1;
