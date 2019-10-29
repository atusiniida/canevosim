#rm(list = ls())
homedir<-dirname(sys.frame(1)$ofile)
setwd(homedir)
source(paste(homedir,"/", "myFunctions.R",sep=""))

if(!any(objects()=="infilePrefix")){
  infilePrefix<-"../out"
}
if(!any(objects()=="outfilePrefix")){
  outfilePrefix<-"../out"
}
if(!any(objects()=="pdf")){
  pdf<-T
}
if(!any(objects()=="png")){
  png<-F
}
if(!any(objects()=="jpeg")){
  jpeg<-F
}


drfile<-paste(infilePrefix,".dr",sep="")
psfile<-paste(infilePrefix,".ps",sep="")
prmfile<-paste(infilePrefix,".prm",sep="")
cellfile<-NULL
subpopcolor<-NULL
if(file.exists(paste(infilePrefix,".df",sep=""))){
  cellfile<-paste(infilePrefix,".df",sep="")
  subpopcolor<-"#54278f"
}
if(file.exists(paste(infilePrefix,".ic",sep=""))){
  cellfile<-paste(infilePrefix,".ic",sep="")
  subpopcolor<- NA
}

###################
#read data
###################
tmp<-read.table(prmfile,sep="=")
prm<-tmp[,2]
names(prm)<-tmp[,1]

D<-resizeMutMatrix(readMutF(drfile), 10)
rownames(D)<-paste("D",1:nrow(D),sep="")
P<-resizeMutMatrix(readMutF(psfile),10)
rownames(P)<-paste("P",1:nrow(P),sep="")

C<-NULL
if(!is.null(cellfile)){
  C<-scan(cellfile)
}


########################
#calculate statistics
########################

stat<-calculateStat(D,prm,P,C)

###################
#print statistics
###################

write(rbind(names(stat),stat),file=paste(outfilePrefix,"stat", sep="."),ncolumns=2,sep="\t")

if(!(pdf | jpeg | png)){
  quit()
}

########################
#visualize genome
########################

if(pdf){
  pdf(paste(outfilePrefix,"pdf", sep="."))
  plotHeatmap(D,P,C,subpopcolor)
  dev.off()
}
if(jpeg){
  jpeg(paste(outfilePrefix,"jpg", sep="."),width = 960, height = 960)
  plotHeatmap(D,P,C,subpopcolor)
  dev.off()
}
if(png){
  png(paste(outfilePrefix,"png", sep="."),width = 960, height = 960)
  plotHeatmap(D,P,C,subpopcolor)
  dev.off()
}

