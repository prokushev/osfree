@echo off

set SLAVE_NAME=os2-vbox-i386
set JENKINS_URL=http://10.8.10.18:8080/
set JNLP_SECRET=858b61416279941c8658a8d6302400a142460c31d33c0bce6a38100f7f6d7ff9

f:
cd \src\osfree\tmp\os2

java -jar \slave.jar -jnlpUrl %JENKINS_URL%computer/%SLAVE_NAME%/slave-agent.jnlp -secret %JNLP_SECRET%
