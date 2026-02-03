
const s = ()=> {
    return new Promise((accpet, reject)=>{
        setTimeout(()=>{
            accpet();
        },1000)
    })
} 


(async () =>{

    for(var i =0 ; i < 10 ; i++) {
        await s()
    console.log(i)
} 


})()