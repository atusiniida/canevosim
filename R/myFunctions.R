
library("gplots")
my.col <- function(n){
  colorpanel(n,high="red",low="gray95")
}

my.dist<-function(x){
  dist(x, method = "manhattan")
}

my.sim<-function(x){
  1-dist(x, method = "manhattan")/ncol(x)
}

my.hclust<-function(x){
  #hclust(x,method="ward")
  hclust(x,method="ward.D2")
}

testBranchedEvolution <-function(x, cutoff){
  tmp <- which(apply(x,1,mean)>cutoff)
  if(length(tmp)>1){
    return(any((x[tmp,])%*%t(x[tmp,])==0))
  }else{
    return(FALSE)
  }
}

readMut<-function(file){
  tmp<-scan(file,what=character(), sep="\n", blank.lines.skip=FALSE)
  tmpList<-strsplit(tmp, " ")
  mut<-unique(unlist(tmpList))
  D<-matrix(0,length(mut),length(tmpList))
  rownames(D)<-mut
  for(i in 1:length(tmpList)){
    D[tmpList[[i]],i]<-1
  }
  if(nrow(D)>1){
    D<-D[names(rev(sort(apply(D,1,sum)))),]
  }
  return(D)
}

readMutF<-function(file){
  D <- NULL
  D <- try(readMut(file), silent = FALSE)
  if (class(D) == "try-error") {
    D <- matrix(0,0,0)                       
  }
  D
}

resizeMutMatrix<-function(x, n){
  if(is.vector(x)){
    tmp<-matrix(0, n-1, length(x))
    rownames(tmp)<-paste("X",1:nrow(tmp),sep="")
    return(rbind(x,tmp ))
  }else if(nrow(x) < n){
    tmp <- matrix(0, n-nrow(x), ncol(x))
    rownames(tmp)<-paste("X",1:nrow(tmp),sep="")
    return(rbind(x, tmp))
  }else{
    return(x)
  }
}

getSubcloneProportionVector<-function(M, filteringCutoff=0.05){
  M[apply(M, 1, mean) < filteringCutoff]<-0
  tmp <-apply(M,2, function(x){paste(x,collapse="")})
  n<-table(factor(tmp))
  n<-rev(sort(n))
  return(n)
}


clonalMutationCutoff <- 0.95
subClonalMutationCutoff <- 0.00

calculateStat<-function(D, prm, P=NULL, C=NULL){
  M<-D
  if(!is.null(P)){
    M<-rbind(M,P)
  }
  
  stat<-NULL
  stat["growthTime"] <- log10(prm["time"])
  if(prm["populationSize"]>0){
    stat["populationSize"] <- log10(prm["populationSize"])
  }else{
    stat["populationSize"] <- 0
  }
  if(ncol(M)==0){
    stat["mutationCountPerCell"] <- NA
    stat["clonalMutationCount"] <- NA
    stat["subClonalMutationCount"] <- NA
    stat["totalMutationCount"] <- NA
    stat["clonalMutationProportion"] <- NA
    stat["subClonalMutationProportion"] <- NA
    stat["branchedEvolution0.05"] <- NA
    stat["branchedEvolution0.1"] <- NA
    stat["shannonDiversity0.05"] <- NA
    stat["simpsonDiversity0.05"] <- NA
    stat["shannonDiversity0.1"] <- NA
    stat["simpsonDiversity0.1"] <- NA
    if(!is.null(C)){
      stat["subPopulationProportion"] <- NA
    }
    return(stat)
  }
  #calculate mutationCountMean
  stat["mutationCountPerCell"]<-mean(apply(M, 2, sum))
  
  #calculate clonalMutationCount
  clonalMutation <- rownames(M)[apply(M, 1, mean) >= clonalMutationCutoff]
  clonalMutationCount <- length(clonalMutation)
  stat["clonalMutationCount"] <- clonalMutationCount
  
  #calculate subClonalrMutationCount
  subClonalMutation <- rownames(M)[apply(M, 1, mean) < clonalMutationCutoff & apply(M, 1, mean) > subClonalMutationCutoff]
  subClonalMutationCount <- length(subClonalMutation)
  stat["subClonalMutationCount"] <- subClonalMutationCount
  
  stat["totalMutationCount"] <- clonalMutationCount + subClonalMutationCount
  
  stat["clonalMutationProportion"] <- clonalMutationCount/(clonalMutationCount + subClonalMutationCount)
  stat["subClonalMutationProportion"] <- subClonalMutationCount/(clonalMutationCount + subClonalMutationCount)
  
  #stat["clonalDivergence"] <- stat["totalMutationCount"]/stat["mutationCountPerCell"]
  
  #calculate populationFitness
  #tmp<-prm["replicationRateIncrease"]**apply(D,2,sum)
  #populationFitness <-  mean(tmp)
  #stat["populationFitness"] <- log10(populationFitness)
  
  stat["branchedEvolution0.05"]<-testBranchedEvolution(D,0.05)
  stat["branchedEvolution0.1"]<-testBranchedEvolution(D,0.1)
  
  #calculate diversity
  library("vegan")
  stat["shannonDiversity0.05"] <- diversity(getSubcloneProportionVector(M,0.05))
  stat["simpsonDiversity0.05"] <-diversity(getSubcloneProportionVector(M,0.05), "simpson")
  stat["shannonDiversity0.1"] <- diversity(getSubcloneProportionVector(M,0.1))
  stat["simpsonDiversity0.1"] <-diversity(getSubcloneProportionVector(M,0.1), "simpson")
  
  if(!is.null(C)){
    stat["subPopulationProportion"] <-length(C[C!=0])/length(C)
  }
  
  return(stat)
}


mutationProfileMatrixSampleSize <- 1000
#mutationProfileMatrixGeneSize <- 1000
mutationProfileMatrixGeneSize <- 300

plotHeatmap<-function(D, P=NULL, C=NULL, subpopcolor="#54278f"){
  M<-D
  if(!is.null(P)){
    M<-rbind(M,P)
  }
  if(ncol(M)==0){
    return(NULL)
  }
  E<-NULL
  if(ncol(M) > mutationProfileMatrixSampleSize){
    tmp<-sample(1:ncol(M), mutationProfileMatrixSampleSize)
    E<-M[,tmp]
    if(!is.null(C)){
     C<-C[tmp] 
    }
  }else{
    E<-M
  }
  E<-E[apply(M,1,sum) > 0,,drop=F]
  
  E<-resizeMutMatrix(E,10)
  tmp<-rev(sort(apply(E,1,mean)))
  if(length(which(tmp>0)) >mutationProfileMatrixGeneSize){
    E2<-E[names(tmp)[1:mutationProfileMatrixGeneSize],]
    rownames(E2)<-names(tmp)[1:mutationProfileMatrixGeneSize]
  }else{
    E2<-E[names(tmp),]
    rownames(E2)<-names(tmp)
  }
  
  G<-rep("gray95", nrow(E2))
  names(G)<-rownames(E2)
  tmp<-grep("D",rownames(E2))
  driverMutation<-tmp[apply(E2[tmp,,drop=F],1,mean)>0]
  clonalMutation <- which(apply(E2, 1, mean) >= clonalMutationCutoff)
  subClonalMutation <- which(apply(E2, 1, mean) < clonalMutationCutoff & apply(E2, 1, mean) >= subClonalMutationCutoff)
  clonalDriver <-intersect(driverMutation, clonalMutation)
  subClonalDriver <- intersect(driverMutation, subClonalMutation)
  clonalNeutral <- setdiff(clonalMutation, driverMutation)
  subClonalNeutral <- setdiff(subClonalMutation,driverMutation)
  
  G[clonalDriver]<-"blue"
  G[subClonalDriver]<-"blue"
  
  #G[clonalDriver]<-"#08519c"
  #G[subClonalDriver]<-"#6baed6"
  #G[clonalNeutral]<-"#006d2c"
  #G[subClonalNeutral]<-"#74c476"
  
  H<-NULL
  col<-c("gray95", "red")
  if(length(unique(as.vector(E2))) == 1){
    if(as.vector(E2)[1] == 1){
      col<-c("red")
    }else{
      col<-c("gray95")
    }
  }
  if(is.null(C)){
    H<-heatmap(E2, scale="none", col=col, distfun=my.dist, hclustfun = my.hclust, RowSideColors=G, labRow=NA, labCol=NA)
  }else{
    G2<-rep("gray95", length(C))
    if(!is.na(subpopcolor)){
      G2[C!=0]<-subpopcolor
    }else{
      library(RColorBrewer)
      k <- length(unique(C[C!=0]))
      if(k>0){
        tmp<-brewer.pal(k, "Dark2")
        for( i in 1:k){
          G2[C==i]<-tmp[i]
        }
      }
    }
    H<-heatmap(E2, scale="none", col=col, distfun=my.dist, hclustfun = my.hclust, RowSideColors=G, ColSideColors=G2, labRow=NA, labCol=NA)
  }
  return(H)
}


