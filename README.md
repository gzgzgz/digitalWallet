# digitalWallet
Insight Data Engineering Code Challenge Submission

# digitalWallet
##Insight Data Engineering Code Challenge Submission

- Author: <b>Zhi Guo</b> 
- Date:   <b>11/10/2016</b>
- Function:  Simulating the function of a digital wallet, identifying potential fraud transactions by friend circle. Allow for 3 different detection levels (direct friends, second degree friends, and fourth degree friends)
- Coding language: c++
- Compilation test environment:  
  - gcc/g++ 4.5, 4.6, 4.7 on RHEL 2.6.32 x86_64 GNU/Linux
  - intel compiler 16.0.1 on RHEL 2.6.32 x86_64 GNU/Linux
  - gcc/g++ 4.6.3 on cygwin/Win 10
  - MSVC++ 10.0 on Win 10

- <b>FIXES 5:51 pm</b> the original run.sh does not couple well with the run_test.sh in the insight_testsuit. It now has been updated. A Makefile has been used to make compilation cleaner.                                 

###Tests included:  
- 1 standard tests (1 line record) provided by InsightData (all 3 detection levels passed)
- 1 customized tests (first 10,000 line small test) by the author (all 3 detection level passed), other tests are too large to upload
                 
###benchmark and performance: 
- <b>constructing the data structure (vector + hash table) from scratch through disk I/O.</b> <br>
                In order to pursue for higher performance, low level I/O functions has been used, and input error detection/handling has been omitted (InsightData guarantees intact format). Using the original batch_payment.csv (286 MB), *3,900,000* records in total, *~25 secs* has been consumed to fully establish the paymo user relationship data structure on a single core CPU. 
- <b>fraud detection performance (1st friend, 2nd friend, and 4th friend).</b> <br>
  Benchmark use all the records (*3,900,000*) in batch_payment.csv to construct the transaction network. Meanwhile, all the records (*3,000,000*) in stream_payment.csv has been put into test<br>
                1st friend    ==>    <b>50,000</b> transactions screening per sec <br>
                2nd friend    ==>    <b>10,000</b> transactions screening per sec <br>
                4th friend    ==>     <b>2,00</b>  transactions screening per sec <br></br>
  memory consumption is constantly below 500 MB for 1st and 2nd friend tests.

###Cache consideration:
Original version of this code has built 2nd, 3rd, 4th friend Most Recently Used (MRU) caches in a purpose of enhancing performance. However, it has been found out that the cache hit ratio is quite low (<0.1%) for ***one day transactions***, where most of the transactions do not have strong correlations in the friend circles. As a result, constantly updating cache also burdens the performance, thus this function has been removed from the submission
                
###Source files (only needs STL library):

**class for implementing all the important functions realted data structure constructions and fraud detections**
- DigitalWallet.cpp 
- DigitalWallet.h 

**class for formatting file input and convert it into transaction records**    
- TransLog.cpp
- TransLog.h <br>

**Main function (entry point)**
- digitalWalletTestMain.cpp

###Extra files for test automation (~~parameter files used by run.sh, must be placed in the parent folder, do not delete~~, it is now done automatically by run.sh):
- para1.in  ==> 1st friend  
- para2.in  ==> 2nd friend
- para3.in  ==> 4th friend




                



