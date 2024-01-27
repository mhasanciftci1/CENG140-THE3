#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "the3.h"

/*
    Create a cache by reading inputs from stdin
*/
Cache* createCache() {
    int i;
    int isFreqNodePlaced;
    int stringCompare;

    /* char* videoName, domainName; */
    Cache* cache;
    CacheNode* node;

    DomainFreqNode* domainFreqNode;
    DomainFreqNode* domainFreqNodeIterator;

    char Line[1000];
    char* token;
    int tokenSize;
    int tokenNum;

    /* Allocate memory for the cache */
    cache = (Cache*)malloc(sizeof(Cache));

    /* Allocate memory for first node */
    cache->head = (CacheNode*)malloc(sizeof(CacheNode));
    cache->tail = cache->head;

    node = cache->head;

    isFreqNodePlaced = 0;
    cache->currentSize = 0;

    scanf("%d %d\n", &cache->cacheLimit, &cache->mediaCount);

    for(i=0; i<cache->mediaCount; i++)
    {

        /* Get line from stdin */
        fgets(Line, 1000, stdin);


        /* First token is the name of the media */
            token = strtok(Line, " ");
            tokenSize = strlen(token);

            /* Allocate area for node's media name */
            node->media.name = (char*)malloc(sizeof(char) * (tokenSize + 1));
            strcpy(node->media.name, token);
            /* Add ending character */
            node->media.name[tokenSize] = '\0';

        /* Second token is the size of the media, it is always an integer */
            token = strtok(NULL, " ");
            tokenNum = atoi(token);
            node->media.size = tokenNum;
            cache->currentSize += node->media.size;

        /* Arrange head and tail with the first pair */
            token = strtok(NULL, " ");
            tokenSize = strlen(token);

            node->domainFreqList = (DomainFreqList*)malloc(sizeof(DomainFreqList));
            /* 
               Allocate memory for first node of domain freq list
               assign resulting pointer to head and the tail
            */
            node->domainFreqList->head = (DomainFreqNode*)malloc(sizeof(DomainFreqNode));
            node->domainFreqList->tail = node->domainFreqList->head;

            /*
               Allocate memory for the name of the domain and 
               assign it to the freq list node
            */
            node->domainFreqList->head->name = (char*)malloc(sizeof(char) * (tokenSize + 1));
            strcpy(node->domainFreqList->head->name, token);
            node->domainFreqList->head->name[tokenSize] = '\0';

            /*
                Obtain the frequency and assign the freq field
            */
            token = strtok(NULL, " ");
            tokenNum = atoi(token);
            node->domainFreqList->head->freq = tokenNum;

            /* Obtain second domain name */
            token = strtok(NULL, " ");

        /* Remaining tokens are groups of two that are domain and freq pairs */
            while(token)
            {
                /* Obtain domain name */
                tokenSize = strlen(token);

                /* Allocate memory for the domain freq node that is going to be added*/
                domainFreqNode = (DomainFreqNode*)malloc(sizeof(DomainFreqNode));

                /* Get domain name */
                domainFreqNode->name = (char*)malloc(sizeof(char) * (tokenSize + 1));
                strcpy(domainFreqNode->name, token);
                domainFreqNode->name[tokenSize] = '\0';

                /* Get domain freq */
                token = strtok(NULL, " ");
                tokenNum = atoi(token);
                domainFreqNode->freq = tokenNum;

                /* 
                    Now traverse the domain frequency list and place new node
                    to the appropriate position
                */
               domainFreqNodeIterator = node->domainFreqList->head;
               while(domainFreqNodeIterator)
               {
                    /* First check frequencies list is sorted in descending order */
                    if(domainFreqNode->freq > domainFreqNodeIterator->freq)
                    {
                        /* Node placed before the iterator */

                        /* 
                           If iterator is at the head
                           new node becomes the head
                        */
                        if(domainFreqNodeIterator == node->domainFreqList->head)
                        {
                            domainFreqNode->next = node->domainFreqList->head;
                            node->domainFreqList->head = domainFreqNode;
                            domainFreqNode->next->prev = domainFreqNode;
                        }
                        /*
                            If iterator is ahead of the head
                        */
                        else
                        {
                            domainFreqNode->prev = domainFreqNodeIterator->prev;
                            domainFreqNodeIterator->prev->next = domainFreqNode;
                            domainFreqNodeIterator->prev = domainFreqNode;
                            domainFreqNode->next = domainFreqNodeIterator;
                        }

                        /* We must stop traversing further */
                        isFreqNodePlaced = 1;
                        break;
                    }
                    else if (domainFreqNode->freq == domainFreqNodeIterator->freq)
                    {
                        /* To break the tie, we compare their names ascii ascending order */
                        stringCompare = strcmp(domainFreqNode->name, domainFreqNodeIterator->name);

                        /*
                            If first string is lower, It will be placed
                            before the iterator
                        */
                        if (stringCompare < 0)
                        {
                            /* 
                                If iterator is at the head
                                new node becomes the head
                            */
                            if(domainFreqNodeIterator == node->domainFreqList->head)
                            {
                                domainFreqNode->next = node->domainFreqList->head;
                                node->domainFreqList->head = domainFreqNode;
                                domainFreqNode->next->prev = domainFreqNode;
                            }
                            /*
                                If iterator is ahead of the head
                            */
                            else
                            {
                                domainFreqNode->prev = domainFreqNodeIterator->prev;
                                domainFreqNodeIterator->prev->next = domainFreqNode;
                                domainFreqNodeIterator->prev = domainFreqNode;
                                domainFreqNode->next = domainFreqNodeIterator;
                            }

                            /* We must stop traversing further */
                            isFreqNodePlaced = 1;
                            break;                            
                        }
                    }
                    if(domainFreqNode->freq < domainFreqNodeIterator->freq)
                    {
                        /* 
                           If iterator is at the tail
                           new node becomes the head
                        */
                        if(domainFreqNodeIterator->next == NULL)
                        {
                            domainFreqNodeIterator->next = domainFreqNode;
                            domainFreqNode->prev = domainFreqNodeIterator;
                            domainFreqNode->next = NULL;

                            break;
                        }
                    }

                    /* Carry on iteration */    
                    domainFreqNodeIterator = domainFreqNodeIterator->next;
               }

               /*
                    If we could not place while iterating, it means that
                    we have to place the new node to the tail
                */
               if(isFreqNodePlaced == 0)
               {
                    node->domainFreqList->tail->next = domainFreqNode;
                    domainFreqNode->prev = node->domainFreqList->tail;
                    node->domainFreqList->tail = domainFreqNode;
               }

                /* Obtain next domain name */
                token = strtok(NULL, " ");
            }

        if(i != cache->mediaCount - 1)
        {
            node->next = (CacheNode*)malloc(sizeof(CacheNode));
            cache->tail = node->next;
            cache->tail->prev = node;
            node = node->next;
        }
        
    }

    return cache;
  
}




void printCache(Cache* cache)
{
    CacheNode* controlnode;
    DomainFreqNode* domainnode;
    controlnode= cache->head;
    if(controlnode!=NULL){
        printf("-------------- Cache Information --------------\n");
        printf("Cache Limit: %d KB\n", cache->cacheLimit);
        printf("Current Size: %d KB\n", cache->currentSize);
        printf("Media Count: %d\n", cache->mediaCount);
        printf("Cache Media Info:\n");
        while(controlnode!=NULL){
            printf("    Name: %s Size: %d KB\n",controlnode->media.name,controlnode->media.size);
            printf("    ------- Media Domain Frequencies -------\n");
            domainnode=controlnode->domainFreqList->head;
            while(domainnode!=NULL){
                printf("        Name: %s        Freq: %d\n", domainnode->name,domainnode->freq);
                domainnode=domainnode->next;
            }
            printf("    ----------------------------------------\n");
            controlnode=controlnode->next;
        }
        printf("----------------------------------------------\n");
    }
    else{
        printf("The Cache is Empty");
    }
}



    /* Adds media to the cache */

CacheNode* addMediaRequest(Cache* cache, Media media, char* domain)
{
    CacheNode* newnode; 
    CacheNode* tailnode;
    CacheNode* cachenode = cache->head;
    
    DomainFreqNode* controllernode;
    DomainFreqNode* secondcontrollernode;
    DomainFreqNode* newdomainnode;
    DomainFreqNode* domainsorter;
    DomainFreqNode* domainfornewmedia;
    while (cachenode != NULL) {
        if (strcmp(cachenode->media.name, media.name) == 0) {

            if (cachenode != cache->head) {
                if (cachenode == cache->tail) {
                    cache->tail = cachenode->prev;
                    cache->tail->next = NULL;
                } 
                else {
                    cachenode->prev->next = cachenode->next;
                    cachenode->next->prev = cachenode->prev;
                }


                cachenode->next = cache->head;
                cache->head->prev = cachenode;
                cachenode->prev = NULL;
                cache->head = cachenode;
            }
            controllernode=cachenode->domainFreqList->head;
            secondcontrollernode=cachenode->domainFreqList->head;
             while(controllernode!=NULL){
                if(strcmp(controllernode->name,domain)==0){
                    controllernode->freq++;
                            break;
                        }
                controllernode=controllernode->next;
                }
             if(controllernode==NULL){ /* segv veren yer başı */
                newdomainnode=(DomainFreqNode*)malloc(sizeof(DomainFreqNode));
                newdomainnode->name=(char*)malloc(50*sizeof(char));
                strcpy(newdomainnode->name,domain);
                newdomainnode->freq=1;
                domainsorter=cachenode->domainFreqList->head;
                while(domainsorter!=NULL){
                    if(domainsorter->freq<newdomainnode->freq || (domainsorter->freq==
                    newdomainnode->freq && strcmp(domainsorter->name,newdomainnode->name)>0)){
                        if(domainsorter->prev==NULL){
                            domainsorter->prev=newdomainnode;
                            newdomainnode->next=domainsorter;
                            cachenode->domainFreqList->head=newdomainnode;
                            break;
                        }
                        else if(domainsorter->next==NULL){
                            domainsorter->prev->next=newdomainnode;
                            newdomainnode->prev=domainsorter->prev;
                            newdomainnode->next=domainsorter;
                            domainsorter->prev=newdomainnode;
                            break;
                        }
                        else{
                            domainsorter->prev->next=newdomainnode;
                            newdomainnode->prev=domainsorter->prev;
                            domainsorter->prev=newdomainnode;
                            newdomainnode->next=domainsorter;
                            break;
                        } 
                    
 
                }
                    domainsorter=domainsorter->next;
                } 
            } /* sonu burası */
        return NULL;
        }
        cachenode=cachenode->next;    

        } 



    
    newnode = (CacheNode*)malloc(sizeof(CacheNode));
    newnode->media.name=(char*)malloc(50*sizeof(char));
    strcpy(newnode->media.name,media.name);
    newnode->media.size = media.size;
    newnode->prev = NULL;
    newnode->next = NULL;
    newnode->domainFreqList = (DomainFreqList*)malloc(sizeof(DomainFreqList));
    domainfornewmedia=(DomainFreqNode*)malloc(sizeof(DomainFreqNode));
    domainfornewmedia->name=(char*)malloc(50*sizeof(char));
    strcpy(domainfornewmedia->name,domain);
    domainfornewmedia->freq=1;
    newnode->domainFreqList->head = domainfornewmedia;
    newnode->domainFreqList->tail = domainfornewmedia;
    newnode->next=cache->head;
    cache->head->prev=newnode;
    cache->head=newnode;
    cache->mediaCount++;
    cache->currentSize+= media.size;
    
    while (cache->currentSize > cache->cacheLimit) {
            
            tailnode = cache->tail;
            cache->tail = tailnode->prev;
            cache->tail->next = NULL;
        
        cache->mediaCount--;
        cache->currentSize -= tailnode->media.size;

        
    }
    return NULL; 
    
}

/*
    Finds the corresponding media's node in the cache
    Return NULL if it is not found
*/
CacheNode* findMedia(Cache* cache, char* name)
{
    CacheNode* controllernode=cache->head;
    while(controllernode->next!=NULL){
        if(strcmp(controllernode->media.name,name)==0){
            return controllernode;
        }
        else{
            controllernode=controllernode->next;
        }
    }
    return NULL;
}

/*
    Finds a media from its name and deletes it's node from the cache
    Do not do anything if media is not found
*/
void deleteMedia(Cache* cache, char* name)
{
    int i=9999;
    DomainFreqNode* domainnode;
    DomainFreqNode* nextnode;
    CacheNode* deletingnode=findMedia(cache,name);
    CacheNode* controllernode=cache->head;
    while(controllernode->next!=NULL){
        if(controllernode==deletingnode){
            if(controllernode->prev==NULL){
                cache->head=controllernode->next;
                cache->head->prev=NULL;
            }
            else if(controllernode->next==NULL){
                cache->tail=controllernode->prev;
                cache->tail->next=NULL;
            }
            else{
                controllernode->prev->next = controllernode->next;
                controllernode->next->prev = controllernode->prev;
            }
            cache->currentSize =cache->currentSize - deletingnode->media.size;
            cache->mediaCount=cache->mediaCount - 1;
            i= cache->mediaCount;
        }
        controllernode = controllernode -> next;
    }
    if(i!=9999){
        domainnode = deletingnode->domainFreqList->head;
        while (domainnode != NULL) {
            nextnode = domainnode->next;
            free(domainnode->name);
            free(domainnode);
            domainnode = nextnode;
        }
        free(deletingnode->domainFreqList);
        free(deletingnode->media.name);
        free(deletingnode);
    } 
}

