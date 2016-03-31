#nairserver dockerfile
#make sure that the presence of compile-time base image
#docker registry address: http://docker.neg
#author: rick.b.liu@newegg.com
#date: 2015/9/9

#dockerfile script usage:
step 1 make nairserver image: 
   docker build -t docker.neg/nairserver:<version> nairserver/
step 2 create and start nairserver container:
   docker run -d -p 8887:8887 -P -e NAIR=GDEV --name=nairserver docker.neg/nairserver:<version>
stop 3 checking nairserver container:
   docker ps | grep nairserver

#dockerfile environment variables:
   -e NAIR=DETAULE (default value, connect to test tairconfigserver platform)
   use more options: DEFUALT、GDEV、GQC、PRD
   
#dockerfile port:
   -p 8887:8887 (container internal port is: 8887)
   use more port: xxxx:8887
