#include "GenerateAutomataTruePass.h"

bool GenerateAutomataTruePass::runOnFunction(Function &F) {
    this->Ctx = &F.getContext();
    this->currentFunction = &F;  

    this->st_currentFunctionName = this->currentFunction->getName();

    for(auto& B: F)
    {

        this->runOnBasicBlock(B, this->Ctx); 
        this->printStateData();

    }

    return false;
}

void GenerateAutomataTruePass::printStateData()
{
    if(this->enableDataBlk)
    {  
        ofstream filest;
        filest.open("automata_list_log.st", std::ios_base::app);
        filest << this->st_currentFunctionName << "@";        
        filest << this->st_numLineBeginBB << "@";                
        filest << this->st_startline << "@";
        filest << this->st_sourceCodeLine << "@";
        if(this->st_isControl){
            filest << this->st_controlCode << "@1@";
        }else{
            filest << "NONE@0@";
        }
        filest << this->st_isEntryPoint << "\n";
        filest.close();
    }

    //To write the automata the flow should be using the function name and its call
}


void GenerateAutomataTruePass::runOnBasicBlock(BasicBlock& B, LLVMContext* Ctx) 
{    
    
    this->firstBlockInst = B.begin();    
    this->st_lastBlockInst = --B.end(); // -- is necessary to avoid the pointer to the next block
    this->enableDataBlk = false;
    bool isCond = false;
    
    // Getting data from BB begin
    DebugInfo debugInfoFi(this->Ctx, (Instruction*)this->firstBlockInst);            
    this->st_numLineBeginBB = debugInfoFi.getLineNumberInt();
    
    //Identifying asserts on analayzed code
    this->identifyAssertLoc(B);   

    if(!this->checkBBHasLError(B))
    {

        // Create a method to get info from branches in the block for condition-true and false     
        isCond = this->isBranchCond(B);
        this->st_isControl = isCond;

        if(B.size() > 1)
        {
            
            if(auto* tI = dyn_cast<TerminatorInst>(&*this->st_lastBlockInst))
            {
                if(tI->getOpcodeName() == "br")
                {
                    --this->st_lastBlockInst;            
                    DebugInfo debugInfoLa(this->Ctx, (Instruction*)this->st_lastBlockInst);            
                    this->st_startline = debugInfoLa.getLineNumberInt();            
                    this->st_sourceCodeLine = this->sourceCodeHelper->getLine(debugInfoLa.getLineNumberInt());

                }else{
                    //empty line in the source code
                    DebugInfo debugInfoLa(this->Ctx, (Instruction*)this->st_lastBlockInst);

                    if(this->sourceCodeHelper->getLine(debugInfoLa.getLineNumberInt()).empty())
                    {
                        int numline = debugInfoLa.getLineNumberInt() - 1;
                        this->st_startline = numline;
                        this->st_sourceCodeLine = this->sourceCodeHelper->getLine(numline);

                    }else{
                        this->st_startline = debugInfoLa.getLineNumberInt();
                        this->st_sourceCodeLine = this->sourceCodeHelper->getLine(debugInfoLa.getLineNumberInt());                                              
                    }

                }
            }
            this->enableDataBlk = true;

        }else{
            //In case this unique instruction be a "br" then we remove this basic block 
            if(auto* tI = dyn_cast<TerminatorInst>(&*this->st_lastBlockInst))
            {
                if(tI->getOpcodeName() != "br")
                {
                    this->st_lastBlockInst = this->firstBlockInst;
                    DebugInfo debugInfoLa(this->Ctx, (Instruction*)this->st_lastBlockInst);
					this->st_startline = debugInfoLa.getLineNumberInt();
                    this->st_sourceCodeLine = this->sourceCodeHelper->getLine(debugInfoLa.getLineNumberInt());
                    this->enableDataBlk = true;
                }
            }

        }
    }

    if(this->enableDataBlk)
    {
        //this->visitInstruction(i);
        if(this->currentFunction->getName() == "main") 
        {
            if(this->countEntryPoint == 1 && this->st_isEntryPoint == false)
            {
                this->st_isEntryPoint = true;
                this->countEntryPoint++;
            }else{
                this->st_isEntryPoint = false;
            }                        
        }
        
    }
}

void GenerateAutomataTruePass::identifyAssertLoc(BasicBlock& B)
{
    for( auto& I : B )
    {
        if(auto* cI = dyn_cast<CallInst>(&I))        
        {   			
			Value* v = cI->getCalledValue();	
			Function* caleeFunction = dyn_cast<Function>(v->stripPointerCasts());			
			//errs() << caleeFunction->getName() << "\n";			
            if(caleeFunction->getName() == "__VERIFIER_assert")
            {
                DebugInfo debugInfoCi(this->Ctx, cI);         
                this->assertListLoc.push_back(debugInfoCi.getLineNumberInt());
            }

        }        
    }
}

// Identify if the block has a branch and define the condition to true and false
bool GenerateAutomataTruePass::isBranchCond(BasicBlock& B)
{
    //errs() << bbName << "\n";
    StringRef actualNameInt;
    std::vector<int>::const_iterator iT;

    for( auto& I : B )
    {      

        if(auto* bI = dyn_cast<ICmpInst>(&I))
        {
            //errs() << this->convertLLPredicatetoXmlText(I) << "\n";
            //
            DebugInfo debugInfoBi(this->Ctx, bI);
            //errs() << debugInfoBi.getLineNumberInt() << "\n";

            //Skiping when the Branch was an assertion
            iT =  std::find(this->assertListLoc.begin(), this->assertListLoc.end(), debugInfoBi.getLineNumberInt()); 
            if ( !(iT != this->assertListLoc.end()) )
            {
                //errs() << "control : " << this->convertLLPredicatetoXmlText(I) << "\n";
                this->st_controlCode = this->convertLLPredicatetoXmlText(I);
                return true;
            }
            /**else{
                //errs() << this->cprogram_path << "\n";
                errs() << debugInfoBi.getLineNumberInt() << "\n";
                errs() << this->sourceCodeHelper->getLine(debugInfoBi.getLineNumberInt()) << "\n";
            }**/
        }


        /**if(auto* tI = dyn_cast<TerminatorInst>(&I))
          {
          if(tI->getOpcodeName() == "br")
          {
        //errs() << tI->getSuccessor(1)->getValueName() << "\n";
        //errs() << tI->successors().begin()->getValueName() << "\n";

        errs() << tI->getOpcodeName() << ":::::::::::::::: \n";
        tI->dump();
        if(tI->getNumSuccessors() > 1)
        {
        BasicBlock::iterator trueCond = tI->getSuccessor(0)->begin();
        DebugInfo debugInfoT(this->Ctx, (Instruction*)trueCond);
        errs() << "True Cond in:" << debugInfoT.getLineNumberInt() << "\n";

        BasicBlock::iterator falseCond = tI->getSuccessor(1)->begin();
        DebugInfo debugInfoF(this->Ctx, (Instruction*)falseCond);
        errs() << "False Cond in:" << debugInfoF.getLineNumberInt() << "\n";

        }

        }
        }**/

        //errs() << I.getOpcodeName() << "\n";
    }

    return false;
}


std::string GenerateAutomataTruePass::convertLLPredicatetoXmlText(Instruction& I)
{
    std::string lvaluep;
    std::string rvaluep;
    std::string predicateInXml;
    std::string fullExpPredicateInXml;
    std::ostringstream osstrtmp;

    if(auto* bI = dyn_cast<ICmpInst>(&I))
    {

        //bI->dump();
        //errs() << bI->getSignedPredicate() << "\n";
        //errs() << this->getPredicateSymOnXmlText(*bI) << "\n";
        predicateInXml = this->getPredicateSymOnXmlText(*bI);
        //errs() << *bI->getOperand(0) << "\n";
        //errs() << bI->getOperand(0)->getValueName() << "\n";

        //Getting left value from predicate 
        if(isa<LoadInst>(bI->getOperand(0))) {
            LoadInst *LD100 = cast<LoadInst>(bI->getOperand(0));
            Value *C100 = LD100->getPointerOperand();
            lvaluep = C100->getName().str(); 
            //errs() << "LVALUEP: " << lvaluep << "\n";

        } else if (ConstantInt* CI = dyn_cast<ConstantInt>(bI->getOperand(0))) {
            if (CI->getBitWidth() <= 32) { //Of course, you can also change it to <= 64 if constIntValue is a 64-bit integer, etc.
                //constIntValue = CI->getSExtValue();
                osstrtmp << CI->getSExtValue();
                rvaluep = osstrtmp.str();
                //errs() << "LVALUEP:" << lvaluep << "\n";
            }
        }


        //Instruction tmpI = (Instruction) &*bI->getOperand(0);
        //errs() << *bI->getOperand(1) << "\n";

        if(isa<LoadInst>(bI->getOperand(1))) {
            LoadInst *LD101 = cast<LoadInst>(bI->getOperand(1));
            Value *C101 = LD101->getPointerOperand(); //HERE COMPILATION ERROR
            rvaluep = C101->getName().str();
            //errs() << "RVALUEP:" << rvaluep << "\n";
        }
        //errs() << bI->getOperand(1)->getName().str() << "\n";
        else if (ConstantInt* CI = dyn_cast<ConstantInt>(bI->getOperand(1))) {
            if (CI->getBitWidth() <= 32) { //Of course, you can also change it to <= 64 if constIntValue is a 64-bit integer, etc.
                //constIntValue = CI->getSExtValue();
                osstrtmp << CI->getSExtValue();
                rvaluep = osstrtmp.str();
                //errs() << "RVALUEP:" << rvaluep << "\n";
            }
        }


        //Generate full predicate expression
        fullExpPredicateInXml = "[" + lvaluep + " " + predicateInXml + " " + rvaluep + "]";
        //errs() << fullExpPredicateInXml << "\n";
        return fullExpPredicateInXml;

    }
    return "[]";
}

std::string GenerateAutomataTruePass::getPredicateSymOnXmlText(ICmpInst& icmpInst)
{
    std::string predicateText = "";

    ICmpInst::Predicate pr = icmpInst.getSignedPredicate();
    if(pr == ICmpInst::ICMP_EQ)
    {
        predicateText="==";
    }else if(pr == ICmpInst::ICMP_NE)
    {
        predicateText="!";
    }else if(pr == ICmpInst::ICMP_SGT)
    {
        predicateText="&gt;";
    }else if(pr == ICmpInst::ICMP_SGE)
    {
        predicateText="&ge;";
    }else if(pr == ICmpInst::ICMP_SLT)
    {
        predicateText="&lt;";
    }else if(pr == ICmpInst::ICMP_SLE)
    {
        predicateText="&le;";
    }
    /**switch(pr){
        case ICmpInst::ICMP_SGT: predicateText="&gt;"; break;
        case ICmpInst::ICMP_SLT: predicateText="&lt;"; break;
        case ICmpInst::ICMP_SGE: predicateText="&ge;"; break;
        case ICmpInst::ICMP_SLE: predicateText="&le;"; break;
    }**/

    return predicateText;

}

//To identify a block with a error location by __VERIFIER_error call function
bool GenerateAutomataTruePass::checkBBHasLError(BasicBlock& nowB)
{
    for( auto& I : nowB )
    {
        if(CallInst* callInst = dyn_cast<CallInst>(&I))
        {
            //errs() << callInst->getCalledFunction()->getName() << "\n";
            Value* v = callInst->getCalledValue();	
			Function* caleeFunction = dyn_cast<Function>(v->stripPointerCasts());
            if(caleeFunction->getName() ==  "__VERIFIER_error")
            {
                return true;
            }
        }
    }
    //std::cout << i.begin << "\n";
    //std::cout << i->getOpcodeName(i->getOpcode()) << std::endl;
    return false;
}

char GenerateAutomataTruePass::ID = 20;
static RegisterPass<GenerateAutomataTruePass> X("generate_automata_true", "Generate Automata True");




