//
// Created by Dominik on 16.05.2018.
//

#ifndef LANGUAGE_INTERPRETER_MATRIXC_PTREE_H
#define LANGUAGE_INTERPRETER_MATRIXC_PTREE_H


class PTree{    // Parser tree - drzewo rozbioru
public:
    PTree(){};
    explicit PTree(Element* e){
        setElement(e);
    }
    void add(PTree* child){
        children.push_back(child);
    }
    void setElement(Element* e){
        element = e;
    }
    void deleteChildren(PTree* tree){
        if(children.size() == 0){
            delete this;
        }else{
            for(auto iter = children.begin(); iter!= children.end(); iter++){
                deleteChildren(*iter);
            }
        }
    }
    const list<PTree *> &getChildren() const {
        return children;
    }
    Element* getElement() const {
        return element;
    }
private:
    std::list<PTree*> children;
    Element* element;

};

#endif //LANGUAGE_INTERPRETER_MATRIXC_PTREE_H
