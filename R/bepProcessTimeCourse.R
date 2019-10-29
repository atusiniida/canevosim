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
pdf<-T
png<-T

###################
#read data
###################

DList<-NULL
PList<-NULL
CList<-NULL
prmTable<-NULL
statTable<-NULL

subpopcolor<-"#54278f"
for(i in 0:100){
  drfile<-paste(infilePrefix,".", i,".dr",sep="")
  psfile<-paste(infilePrefix,".", i,".ps",sep="")
  prmfile<-paste(infilePrefix,".", i,".prm",sep="")
  cellfile<-NULL
  if(file.exists(paste(infilePrefix,".", i,".df",sep=""))){
    cellfile<-paste(infilePrefix,".", i,".df",sep="")
    subpopcolor<-"#54278f"
  }
  if(file.exists(paste(infilePrefix,".", i,".ic",sep=""))){
    cellfile<-paste(infilePrefix,".", i,".ic",sep="")
    subpopcolor<- NA
  }
  
  if(file.exists(prmfile)){
    tmp<-read.table(prmfile,sep="=")
    prm<-tmp[,2]
    names(prm)<-tmp[,1]
    D<-resizeMutMatrix(readMut(drfile), 10)
    rownames(D)<-paste("D",1:nrow(D),sep="")
    P<-resizeMutMatrix(readMut(psfile),10)
    rownames(P)<-paste("P",1:nrow(P),sep="")
    C<-NULL
    if(!is.null(cellfile)){
      C<-scan(cellfile)
    }
    DList<-c(DList, list(D))
    PList<-c(PList, list(P))
    CList<-c(CList, list(C))
    prmTable<-cbind(prmTable, prm)
  }else{
    break
  }  
}
N<-NULL
time<-NULL
for(i in 1:length(DList)){
  if(prmTable["populationSize",i]>=10){
    N<-c(N, i); 
    time<-c(time, prmTable["time",i])
    stat<-calculateStat(DList[[i]], prmTable[,i], PList[[i]],CList[[i]])
    statTable<-cbind(statTable,stat)
    write(rbind(names(stat),stat),file=paste(outfilePrefix,i-1,"stat", sep="."),ncolumns=2,sep="\t")
  }
}

Y<-rownames(statTable)[rownames(statTable)!="growthTime"]

plotTimeCourse<-function(y){
  if(y == "populationSize"){
    gcfile<-paste(infilePrefix,".gc",sep="")
    tmp<-read.table(gcfile)
    t<-tmp[,1]
    p<-log10(tmp[,2])
    plot(time, statTable[y,], xlab="time", ylab=y,cex=2, xlim=range(t), ylim=range(p), ann=F , type="n")
    lines(t, p, lty=2)
    par(new=T)   
    plot(time, statTable[y,], xlab="time", ylab=y,cex=2, xlim=range(t), ylim=range(p))
  }else{
    plot(time, statTable[y,], xlab="time", ylab=y,cex=2, type="b", lty=2)
  }
} 

dev.on<-function(type,x){
  if(type=="pdf"){
    pdf(paste(outfilePrefix,x,"pdf", sep="."))
  }
  if(type=="jpeg"){
    jpeg(paste(outfilePrefix,x,"jpg", sep="."),width = 960, height = 960)
  }
  if(type=="png"){
    png(paste(outfilePrefix,x,"png", sep="."),width = 960, height = 960)
  }
}


if(!(pdf | jpeg | png)){
  quit()
}


if(pdf){
  for(i in Y){
    dev.on("pdf",i)
    plotTimeCourse(i)
    dev.off()
  }
  
  for(i in N){
    dev.on("pdf",i-1)
    plotHeatmap(DList[[i]],PList[[i]],CList[[i]],subpopcolor)
    dev.off()
  }
}
if(jpeg){
  for(i in Y){
    dev.on("jpeg",i)
    plotTimeCourse(i)
    dev.off()
  }
  
  for(i in N){
    dev.on("jpeg",i-1)
    plotHeatmap(DList[[i]],PList[[i]],CList[[i]],subpopcolor)
    dev.off()
  }
}
if(png){
  for(i in Y){
    dev.on("png",i)
    plotTimeCourse(i)
    dev.off()
  }
  
  for(i in N){
    dev.on("png",i-1)
    plotHeatmap(DList[[i]],PList[[i]],CList[[i]],subpopcolor)
    dev.off()
  }
}

